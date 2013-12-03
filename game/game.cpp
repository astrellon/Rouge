#include "game.h"

#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include <gfx/gfx_layer.h>
#include <gfx/gfx_engine.h>

#include <ui/mouse_manager.h>
#include <ui/ui_game_hud.h>
#include <ui/ui_inspector.h>
#include <ui/keyboard_manager.h>

#include <lua/wrappers/game/lua_map.h>

#include <sys/game_system.h>

#include <util/data_table.h>
#include <util/data_string.h>
#include <util/data_number.h>

#include "engine.h"
#include "map.h"
#include "player_hand.h"
#include "character.h"
#include "door.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int Game::LUA_ID = LUA_ID_GAME;
	const char *Game::LUA_TABLENAME = LUA_TABLE_GAME;

	Game::Game(Engine *engine) :
		mEngine(engine),
		mCurrentMap(nullptr),
		mActiveObjects(nullptr),
		mGameLayer(new gfx::Layer()),
		mBackground(new gfx::Layer()),
		mItemLayer(new gfx::Layer()),
		mCharacterLayer(new gfx::Layer()),
		mForeground(new gfx::Layer()),
		mStarted(false),
		mGameTickPosition(0),
		mCurrentTickDt(0.0f),
		mGameTickPaused(true),
		mEditorMode(false)
	{
		if (engine == nullptr)
		{
			mEngine = Engine::getEngine();
		}
		mGameLayer->setName("Game->GameLayer");
		mGameLayer->setInteractive(true);

		mBackground->setName("Background");
		mBackground->setInteractive(true);
		mGameLayer->addChild(mBackground.get());

		mItemLayer->setName("ItemLayer");
		mItemLayer->setInteractive(true);
		mGameLayer->addChild(mItemLayer.get());

		mCharacterLayer->setName("CharacterLayer");
		mCharacterLayer->setInteractive(true);
		mGameLayer->addChild(mCharacterLayer.get());

		mForeground->setName("Foreground");
		mForeground->setInteractive(false);
		mGameLayer->addChild(mForeground.get());

		ui::GameObjectEvent::getManager()->addEventListener("obj_click", this);
	}
	Game::~Game()
	{
		deinit();
	}

	void Game::deinit()
	{
		mCamera.followObject(nullptr);
		mGameLayer->deinit();
		{
			DialogueMap dialogue = mDialogueMap;
			for (auto iter = dialogue.begin(); iter != dialogue.end(); ++iter)
			{
				delete iter->second;
			}
			mDialogueMap.clear();
		}
		{
			MapMap maps = mMaps;
			for (auto iter = maps.begin(); iter != maps.end(); ++iter)
			{
				iter->second->deinit();
			}
			mMaps.clear();
		}
	}
	
	Map *Game::getMapLua(const char *mapName)
	{
		return getMapLua(std::string(mapName));
	}
	Map *Game::getMapLua(const std::string &mapName)
	{
		MapMap::iterator iter = mMaps.find(mapName);
		if (iter != mMaps.end())
		{
			return iter->second.get();
		}

		std::stringstream ss;
		ss << "data/maps/" << mapName << ".lua";
		//LuaState lua(false);
		lua::LuaState &lua = mEngine->getLua();
		int loadError = luaL_loadfile(lua, ss.str().c_str());
		if (loadError)
		{
			std::stringstream errss;
			errss << "Unable to load map '" << mapName << "', using the path '";
			errss << ss.str() << '\''; 
			am_log("MAP", errss);
			lua.logStack("MAPLUA");
			return nullptr;
		}
		try
		{
			lua.call(0, 1);

			Map *map = lua::castUData<Map>(lua, -1);

			lua.pop(1);

			if (map)
			{
				map->setFilename(mapName.c_str());
				mMaps[mapName] = map;
			}
			return map;
		}
		catch (std::runtime_error err)
		{
			am_log("MAP", err.what());
		}
		return nullptr;
	}

	Map *Game::getCurrentMap()
	{
		return mCurrentMap.get();
	}

	void Game::onEvent(ui::MouseEvent *e)
	{
		gfx::TileRenderer *target = dynamic_cast<gfx::TileRenderer *>(e->getEventTarget());
		GameObject *obj = dynamic_cast<GameObject *>(e->getOriginalTarget());
		if (!mCurrentMap)// || ((!target || target != mCurrentMap->getTileRenderer() && !obj)))
		{
			return;
		}
		if (!target && !obj)
		{
			return;
		}

		fireEvent<ui::MouseEvent>(e);

		if (e->getMouseEventType() != ui::Mouse::MOUSE_UP || mEditorMode)
		{
			return;
		}

		//float localX = static_cast<float>(e->getLocalMouseX());
		//float localY = static_cast<float>(e->getLocalMouseY());
		float localX = 0.0f;
		float localY = 0.0f;
		mItemLayer->getScreenToLocal(e->getMouseX(), e->getMouseY(), localX, localY);

		if (localX < 0 || localY < 0 ||
			localX > mCurrentMap->getWidth() ||
			localY > mCurrentMap->getHeight())
		{
			return;
		}
		if (e->getMouseButton() == ui::Mouse::LEFT_BUTTON)
		{
			PlayerHand *hand = PlayerHand::getPlayerHand();
			if (hand && hand->getInhand() != nullptr)
			{
				if (mMainCharacter->dropItem(hand->getInhand(), localX, localY) == base::SUCCESS)
				{
					Item *inhand = hand->getInhand();
					//inhand->setInteractive(true);
					mItemLayer->addChild(inhand);
					hand->setInhand(nullptr);
				}
				return;
			}
		}

		ObjectList clickedOn;
		if (mActiveObjects)
		{
			ObjectList::iterator iter;
			for (iter = mActiveObjects->begin(); iter != mActiveObjects->end(); ++iter)
			{
				GameObject *obj = iter->get();
				float x = obj->getPositionX();
				float y = obj->getPositionY();
				if (localX >= x && localY >= y &&
					localX < x + obj->getWidth() &&
					localY < y + obj->getHeight())
				{
					clickedOn.push_back(obj);
				}
			}
		}

		if (e->getMouseButton() == ui::Mouse::LEFT_BUTTON)
		{
			if (clickedOn.size() > 0 && mMainCharacter->canReachGameObject(clickedOn[0]))
			{
				clickedOn[0]->interactWith(mMainCharacter, false);
			}
			else
			{
				mMainCharacter->setDestination(localX, localY);
			}
		}
		else if (e->getMouseButton() == ui::Mouse::RIGHT_BUTTON)
		{
			ui::GameHud *gameHud = Engine::getEngine()->getGameHud();
			if (gameHud)
			{
				int gridX = static_cast<int>(localX * Engine::getEngine()->getGridSizeResp());
				int gridY = static_cast<int>(localY * Engine::getEngine()->getGridSizeResp());

				TileInstance *instance = mCurrentMap->getTileInstance(gridX, gridY);
				ui::Inspector *inspector = gameHud->getInspector();
				inspector->setTileInstance(instance);

				inspector->clearGameObjects();
				inspector->addGameObjects(clickedOn);
			}
		}
	}
	void Game::onEvent(ui::GameObjectEvent *e)
	{
		if (!e)
		{
			return;
		}

		//std::stringstream ss;
		//ss << "Clicked on: " << e->getGameObject()->getName();
		//am_log("OBJ", ss);
		/*GameObject *obj = e->getGameObject();
		if (obj->getGameObjectType() == GameObject::ITEM)
		{
			Item *item = dynamic_cast<Item *>(obj);
			if (!item)
			{
				return;
			}
			if (item->getItemLocation() != Item::GROUND)
			{
				return;
			}
		}*/
	}

	bool Game::addMap(Map *map)
	{
		if (map && map->getName().size() > 0)
		{
			mMaps[map->getName()] = map;
			map->setGamePartof(this);
			return true;
		}
		return false;
	}
	bool Game::removeMap(Map *map)
	{
		if (map)
		{
			MapMap::iterator iter;
			for (iter = mMaps.begin(); iter != mMaps.end(); ++iter)
			{
				if (iter->second.get() == map)
				{
					map->setGamePartof(nullptr);
					mMaps.erase(iter);
					return true;
				}
			}
		}
		return false;
	}
	bool Game::removeMap(const char *mapName)
	{
		if (mapName && mapName[0] != '\0')
		{
		std::string mapStr(mapName);
			MapMap::iterator iter;
			for (iter = mMaps.begin(); iter != mMaps.end(); ++iter)
			{
				if (iter->first.compare(mapStr) == 0)
				{
					mMaps.erase(iter);
					return true;
				}
			}
		}
		return false;
	}
	void Game::removeAllMaps()
	{
		mMaps.clear();
	}

	bool Game::hasMap(const char *mapName)
	{
		if (mapName == nullptr || mapName[0] == '\0')
		{
			return false;
		}
		MapMap::iterator iter = mMaps.find(std::string(mapName));
		if (iter == mMaps.end())
		{
			return false;
		}
		return true;
	}

	
	void Game::setCurrentMap(Map *map, bool addMap)
	{
		if (mActiveObjects)
		{
			for (auto iter = mActiveObjects->begin(); iter != mActiveObjects->end(); ++iter)
			{
				sfx::ISource *source = (*iter)->getSource(false);
				if (source)
				{
					source->stop();
				}
			}
		}
		mBackground->clear();
		mItemLayer->clear();
		mCharacterLayer->clear();
		mForeground->clear();
		mCamera.followObject(nullptr);

		if (mCurrentMap)
		{
			mCurrentMap->getTileRenderer()->removeEventListener(ui::Mouse::MOUSE_UP, this);
			mCurrentMap->getTileRenderer()->removeEventListener(ui::Mouse::MOUSE_DOWN, this);
			mCurrentMap->getTileRenderer()->removeEventListener(ui::Mouse::MOUSE_MOVE, this);
			mBackground->removeEventListener(ui::Mouse::MOUSE_UP, this);
			mItemLayer->removeEventListener(ui::Mouse::MOUSE_UP, this);
			mCharacterLayer->removeEventListener(ui::Mouse::MOUSE_UP, this);
		}

		mCurrentMap = map;
		if (map)
		{
			mBackground->addChild(map->getBackground());
			mForeground->addChild(map->getForeground());
			mActiveObjects = map->getObjects();

			setTickPositionMainChar();
			map->calcAllTileEdgeValues();

			mCurrentMap->getTileRenderer()->addEventListener(ui::Mouse::MOUSE_UP, this);
			mCurrentMap->getTileRenderer()->addEventListener(ui::Mouse::MOUSE_DOWN, this);
			mCurrentMap->getTileRenderer()->addEventListener(ui::Mouse::MOUSE_MOVE, this);
			mBackground->addEventListener(ui::Mouse::MOUSE_UP, this);
			mItemLayer->addEventListener(ui::Mouse::MOUSE_UP, this);
			mCharacterLayer->addEventListener(ui::Mouse::MOUSE_UP, this);
			
			if (mActiveObjects)
			{
				ObjectList::iterator iter;
				for (iter = mActiveObjects->begin(); iter != mActiveObjects->end(); ++iter)
				{
					addGameObject(iter->get());
				}
			}
			else
			{
				std::stringstream errss;
				errss << "Map (" << map->getName() << ") return a nullptr object list.";
				am_log("MAP", errss);
			}
			if (addMap && map->getName().size() > 0)
			{
				mMaps[map->getName()] = map;
				map->setGamePartof(this);
			}
		}
	}
	void Game::setCurrentMap(const char *mapName)
	{
		setCurrentMap(getMapLua(mapName));
	}
	void Game::setCurrentMap(const std::string &mapName)
	{
		setCurrentMap(getMapLua(mapName));
	}

	void Game::addGameObject(GameObject *obj)
	{
		if (obj->getGameObjectType() == GameObject::CHARACTER)
		{
			dynamic_cast<Character *>(obj)->clearDestination();
			mCharacterLayer->addChild(obj);
			dynamic_cast<Character *>(obj)->clearDestination();
		}
		else if (obj->getGameObjectType() == GameObject::ITEM)
		{
			mItemLayer->addChild(obj);
		}
		else
		{
			mBackground->addChild(obj);
		}
		sfx::ISource *source = obj->getSource(false);
		if (source)
		{
			source->play();
		}
	}

	bool Game::addGameObjectToMap(GameObject *object)
	{
		if (mCurrentMap.get() == nullptr)
		{
			am_log("MAP", "Unable to add game object to null current map");
			return false;
		}
		return mCurrentMap->addGameObject(object);
	}
	bool Game::removeGameObjectFromMap(GameObject *object)
	{
		if (mCurrentMap.get() == nullptr)
		{
			am_log("MAP", "Unable to remove game object from null current map");
			return false;
		}
		if (mCurrentMap->removeGameObject(object))
		{
			if (strcmp(object->getGameObjectTypeName(), "character") == 0)
			{
				mCharacterLayer->removeChild(object);
			}
			else
			{
				mItemLayer->removeChild(object);
			}
			return true;
		}
		return false;
	}
	bool Game::hasGameObjectInMap(GameObject *object) const
	{
		if (mCurrentMap.get() == nullptr)
		{
			am_log("MAP", "Unable to search for game object with null current map");
			return false;
		}
		return mCurrentMap->hasGameObject(object);
	}

	void Game::moveObjectToMap(GameObject *object, const char *mapName, float x, float y, bool setAsCurrent)
	{
		if (!object)
		{
			return;
		}
		moveObjectToMap(object, getMapLua(mapName), x, y, setAsCurrent);
	}
	void Game::moveObjectToMap(GameObject *object, Map *map, float x, float y, bool setAsCurrent)
	{
		if (!object || !map)
		{
			return;
		}
		Map *currentMap = object->getMap();
		if (currentMap)
		{
			if (currentMap->removeGameObject(object) && currentMap == mCurrentMap.get())
			{
				mCharacterLayer->removeChild(object);
			}
		}
		bool following = false;
		if (setAsCurrent)
		{
			following = mCamera.getFollowing() == object;
			setCurrentMap(map);
		}
		object->setMap(map);
		object->setLocation(x, y);
		if (following)
		{
			mCamera.followObject(object);
		}
		if (map)
		{
			if (map->addGameObject(object) && map == mCurrentMap.get())
			{
  				addGameObject(object);
			}
		}
	}
	void Game::moveObjectToMapGrid(GameObject *object, const char *mapName, int x, int y, bool setAsCurrent)
	{
		float gx = Engine::getEngine()->getGridSize() * static_cast<float>(x);
		float gy = Engine::getEngine()->getGridSize() * static_cast<float>(y);
		moveObjectToMap(object, mapName, gx, gy, setAsCurrent);
	}
	void Game::moveObjectToMapGrid(GameObject *object, Map *map, int x, int y, bool setAsCurrent)
	{
		float gx = Engine::getEngine()->getGridSize() * static_cast<float>(x);
		float gy = Engine::getEngine()->getGridSize() * static_cast<float>(y);
		moveObjectToMap(object, map, gx, gy, setAsCurrent);
	}

	Camera *Game::getCamera()
	{
		return &mCamera;
	}

	gfx::Layer *Game::getGameLayer()
	{
		return mGameLayer.get();
	}
	gfx::Layer *Game::getBackground()
	{
		return mBackground.get();
	}
	gfx::Layer *Game::getItemLayer()
	{
		return mItemLayer.get();
	}
	gfx::Layer *Game::getCharacterLayer()
	{
		return mCharacterLayer.get();
	}
	gfx::Layer *Game::getForeground()
	{
		return mForeground.get();
	}

	void onRetain2(base::IManaged *obj)
	{
		std::stringstream ss;
		ss << "Retained " << obj->getReferenceCounter();
		am_log("CHAR", ss);
	}
	void onRelease2(base::IManaged *obj)
	{
		std::stringstream ss;
		ss << "Released " << obj->getReferenceCounter();
		am_log("CHAR", ss);
	}

	void Game::setMainCharacter(Character *character)
	{
		if (character)
		{
			//character->onRetain(&onRetain2);
			//character->onRelease(&onRelease2);
		}
		mMainCharacter = character;
	}
	Character *Game::getMainCharacter()
	{
		return mMainCharacter;
	}

	std::string Game::getLoadingFile() const
	{
		return mLoadingFiles.back();
	}

	void Game::addCharDefinition(Character *character, const char *name)
	{
		addDefinition<Character>(character, mCharDefinitions, name);
	}
	Character *Game::getCharDefinition(const char *name)
	{
		return getDefinition<Character>(mCharDefinitions, name);
	}

	void Game::addItemDefinition(Item *item, const char *name)
	{
		addDefinition<Item>(item, mItemDefinitions, name);
	}
	Item *Game::getItemDefinition(const char *name)
	{
		return getDefinition<Item>(mItemDefinitions, name);
	}

	template <>
	Character *Game::create<Character>(const char *definitionName, bool force)
	{
		Character *def = getCharDefinition(definitionName);
		if (def)
		{
			return new Character(*def);
		}
		if (force)
		{
			return new Character();
		}
		return nullptr;
	}
	template <>
	Item *Game::create<Item>(const char *definitionName, bool force)
	{
		Item *def = getItemDefinition(definitionName);
		if (def)
		{
			return new Item(*def);
		}
		if (force)
		{
			return new Item();
		}
		return nullptr;
	}
	
	void Game::update(float dt)
	{
		if (mActiveObjects)
		{
			for (auto iter = mActiveObjects->begin(); iter != mActiveObjects->end(); ++iter)
			{
				iter->get()->update(dt);
			}
		}
		mCamera.update(dt);
	}
	void Game::onGameTick()
	{
		if (mActiveObjects == nullptr || mGameTickPaused || mEditorMode)
		{
			return;
		}
		if (mGameTickPosition >= mActiveObjects->size())
		{
			mGameTickPosition = 0;
		}

		if (mGameTickPosition == 0)
		{
			// Start of new turn.
		}

		GameObject *obj = mActiveObjects->at(mGameTickPosition);
		obj->onGameTick(mCurrentTickDt);
		mGameTickPosition++;
	}
	void Game::setCurrentGameTickLength(float dt)
	{
		mCurrentTickDt = dt;
	}
	void Game::endGameTick()
	{

	}

	GameObject *Game::getGameObject(const char *id) const
	{
		if (id == nullptr || id[0] == '\0')
		{
			return nullptr;
		}
		GameObjectIdMap::const_iterator iter = mGameObjects.find(std::string(id));
		if (iter != mGameObjects.end())
		{
			return iter->second;
		}
		return nullptr;
	}
	bool Game::registerGameObject(GameObject *obj)
	{
		if (obj == nullptr)
		{
			return false;
		}
		const char *id = obj->getGameId();
		if (id == nullptr || id[0] == '\0')
		{
			return false;
		}
		mGameObjects[string(id)] = obj;
		return true;
	}
	void Game::deregisterGameObject(const char *id)
	{
		if (id && id[0] != '\0')
		{
			std::string idStr(id);
			GameObjectIdMap::iterator iter = mGameObjects.find(idStr);
			if (iter != mGameObjects.end())
			{
				mGameObjects.erase(iter);
			}
		}
	}
	void Game::deregisterGameObject(GameObject *obj)
	{
		if (obj == nullptr)
		{
			return;
		}
		GameObjectIdMap::iterator iter = mGameObjects.find(obj->getGameId());
		if (iter != mGameObjects.end())
		{
			mGameObjects.erase(iter);
		}
	}

	bool Game::addDialogue(Dialogue *dialogue)
	{
		if (dialogue)
		{
			std::string id = dialogue->getId();
			DialogueMap::const_iterator iter = mDialogueMap.find(id);
			if (iter == mDialogueMap.end())
			{
				mDialogueMap[id] = dialogue;
				return true;
			}
		}
		return false;
	}
	bool Game::removeDialogue(const char *id)
	{
		if (id != nullptr)
		{
			DialogueMap::const_iterator iter = mDialogueMap.find(std::string(id));
			if (iter == mDialogueMap.end())
			{
				mDialogueMap.erase(iter);
				return true;
			}
		}
		return false;
	}
	void Game::removeAllDialogue()
	{
		mDialogueMap.clear();
	}
	Dialogue *Game::getDialogue(const char *id)
	{
		if (id != nullptr)
		{
			DialogueMap::iterator iter = mDialogueMap.find(std::string(id));
			if (iter != mDialogueMap.end())
			{
				return iter->second;
			}
		}
		return nullptr;
	}

	void Game::getAvailableDialogues(std::vector<Dialogue *> &result, const GameObject *talker, const GameObject *talkedTo)
	{
		if (talker == nullptr || talkedTo == nullptr || 
			talker->getDialogueComp() == nullptr || talkedTo->getDialogueComp() == nullptr)
		{
			return;
		}

		const Character::SubjectMap &unlocked = talker->getDialogueComp()->getUnlockedSubjects();
		const Character::SubjectMap &available = talkedTo->getDialogueComp()->getDialoguesAvailable();
		Character::SubjectMap::const_iterator iter;
		for (iter = available.begin(); iter != available.end(); ++iter)
		{
			Dialogue *dialogue = getDialogue(iter->first.c_str());
			if (dialogue == nullptr)
			{
				continue;
			}
			if (dialogue->getUnlockFlag() == Dialogue::UNLOCK_NONE)
			{
				result.push_back(dialogue);
			}
			else if (dialogue->getUnlockFlag() == Dialogue::UNLOCK_LOCKED)
			{
				// If the subject of the dialogue is in the unlocked map and is true.
				Character::SubjectMap::const_iterator iter = unlocked.find(std::string(dialogue->getSubject()));
				if (iter != unlocked.end() && !iter->second)
				{
					result.push_back(dialogue);
				}
			}
		}
	}

	bool Game::addQuest(Quest *quest)
	{
		if (quest == nullptr)
		{
			return false;
		}
		QuestMap::const_iterator iter = mQuestMap.find(quest->getQuestId());
		if (iter == mQuestMap.end())
		{
			mQuestMap[quest->getQuestId()] = quest;
			return true;
		}
		return false;
	}
	bool Game::removeQuest(const char *questId)
	{
		if (questId == nullptr || questId[0] == '\0')
		{
			return false;
		}
		QuestMap::const_iterator iter = mQuestMap.find(std::string(questId));
		if (iter != mQuestMap.end())
		{
			mQuestMap.erase(iter);
			return true;
		}
		return  false;
	}
	Quest *Game::getQuest(const char *questId)
	{
		if (questId == nullptr || questId[0] == '\0')
		{
			return nullptr;
		}
		QuestMap::const_iterator iter = mQuestMap.find(std::string(questId));
		if (iter != mQuestMap.end())
		{
			return iter->second;
		}
		return nullptr;
	}

	void Game::setGameTickPaused(bool paused)
	{
		mGameTickPaused = paused;
	}
	bool Game::isGameTickPaused() const
	{
		return mGameTickPaused;
	}

	void Game::setEditorMode(bool editor)
	{
		mEditorMode = editor;
	}
	bool Game::isEditorMode() const
	{
		return mEditorMode;
	}

	void Game::startGame()
	{
		mStarted = true;
		setGameTickPaused(false);
		setTickPositionMainChar();
		base::Handle<ui::Event> e(new ui::Event("startGame"));
		fireEvent<ui::Event>(e);
	}
	bool Game::hasStarted() const
	{
		return mStarted;
	}

	void Game::setScenarioName(const char *scenarioName)
	{
		mScenarioName = scenarioName;
	}
	const char *Game::getScenarioName() const
	{
		return mScenarioName.c_str();
	}

	void Game::setGenericDeadGraphic(gfx::Sprite *graphic)
	{
		// Should alert things that this has changed.
		mGenericDeadGraphic = graphic;
	}
	gfx::Sprite *Game::getGenericDeadGraphic() const
	{
		return mGenericDeadGraphic;
	}

	void Game::saveGame(const char *saveName)
	{
		if (saveName == nullptr || saveName[0] == '\0')
		{
			return;
		}

		std::string dir = "saves/";
		dir += saveName;
		dir += '/';
		std::string fileName = dir;
		fileName += "main.lua";
		
		sys::GameSystem::getGameSystem()->createDirectory(dir.c_str());

		std::ofstream output(fileName);
		if (output)
		{
			// Save file already exists, do something?
			// PANIC! D:
		}
		output << "-- Rouge Saved Game\n";
		
		output << "-- This contains data about the current game.\n\n";
		base::Handle<data::IData> gameData(saveGameData());
		if (gameData)
		{
			output << "game = " << gameData->toLua();
		}
		
		output << "\n-- List of characters based on gameid\n";
		output << "characters = ";
		base::Handle<data::Table> list(new data::Table());
		for (auto iter = mGameObjects.begin(); iter != mGameObjects.end(); ++iter)
		{
			gameData = iter->second->serialise();
			if (gameData)
			{
				list->push(gameData);
			}
		}
		output << list->toLua();
		output << "\n-- List of currently active quests\n";
		output << "quests = ";

		list = new data::Table();
		for (auto iter = mQuestMap.begin(); iter != mQuestMap.end(); ++iter)
		{
			gameData = iter->second->serialise();
			if (gameData)
			{
				list->at(iter->first, gameData);
			}
		}
		output << list->toLua();
		output << "\n";

		output.close();
	}
	int Game::loadGame(const char *saveName)
	{
		if (saveName == nullptr || saveName[0] == '\0')
		{
			return 0;
		}

		std::string dir = "saves/";
		dir += saveName;
		dir += '/';
		std::string fileName = dir;
		fileName += "main.lua";

		if (!sys::GameSystem::getGameSystem()->isFile(fileName.c_str()))
		{
			return 0;
		}

		mLoadingState = new LoadingState();

		lua::LuaState lua(false);
		if (!lua.loadFile(fileName.c_str()))
		{
			lua.logStack("LOADERR");
			return -1;
		}

		if (lua.getGlobal("game"))
		{
			base::Handle<data::IData> gameDataObj(data::IData::fromLua(lua, -1));
			base::Handle<data::Table> gameData(dynamic_cast<data::Table *>(gameDataObj.get()));
			if (!gameData)
			{
				std::stringstream ss;
				ss << "Load file 'game' object was a '" << gameDataObj->typeName() << "' not an 'Map'";
				am_log("LOADERR", ss);
				lua.close();
				return -2;
			}

			loadGameData(gameData);
			lua.pop(1);
		}
		if (lua.getGlobal("characters"))
		{
			base::Handle<data::IData> charDataObj(data::IData::fromLua(lua, -1));
			base::Handle<data::Table> charData(dynamic_cast<data::Table *>(charDataObj.get()));
			if (!charData)
			{
				std::stringstream ss;
				ss << "Load file 'characters' object was a '" << charDataObj->typeName() << "' not a 'Table'";
				am_log("LOADERR", ss);
				lua.close();
				return -3;
			}

			for (auto iter = charData->beginArray(); iter != charData->endArray(); ++iter)
			{
				base::Handle<Character> newChar(new Character());
				newChar->deserialise(mLoadingState, iter->get());
				registerGameObject(newChar);
			}
			lua.pop(1);
		}
		if (lua.getGlobal("quests"))
		{
			base::Handle<data::IData> questDataObj(data::IData::fromLua(lua, -1));
			base::Handle<data::Table> questData(dynamic_cast<data::Table *>(questDataObj.get()));
			if (!questData)
			{
				std::stringstream ss;
				ss << "Load file 'quests' object was a '" << questDataObj->typeName() << "' not a 'Table'";
				am_log("LOADERR", ss);
				lua.close();
				return -4;
			}

			for (auto iter = questData->beginMap(); iter != questData->endMap(); ++iter)
			{
				base::Handle<Quest> newQuest(new Quest(iter->first.c_str()));
				newQuest->deserialise(mLoadingState, iter->second.get());
				addQuest(newQuest);
			}
			lua.pop(1);
		}

		mLoadingState->postLoad(this);
		
		return 1;
	}

	LoadingState *Game::getLoadingState()
	{
		return mLoadingState.get();
	}

	data::IData *Game::saveGameData()
	{
		data::Table *data = new data::Table();
		data->comment("This contains data about the current game.");
		if (mMainCharacter)
		{
			data->at("mainChar", mMainCharacter->getGameId());
		}
		if (mCurrentMap)
		{
			data->at("currentMap", mCurrentMap->getFilename());
		}
		data->at("scenarioName", mScenarioName.c_str());
		IAttributeData::serialise(data);

		return data;
	}

	void Game::loadGameData(data::Table *obj)
	{
		if (!obj)
		{
			return;
		}
		base::Handle<data::Table> hobj(obj);

		base::Handle<data::String> str(obj->at<data::String>("mainChar"));
		if (str)
		{
			mLoadingState->setMainCharacter(str->string());
		}

		str = obj->at<data::String>("currentMap");
		if (str)
		{
			mLoadingState->setCurrentMap(str->string());
		}

		str = obj->at<data::String>("scenarioName");
		if (str)
		{
			mScenarioName = str->string();
		}

		IAttributeData::deserialise(mLoadingState, hobj);
	}

	void Game::setTickPositionMainChar()
	{
		if (mActiveObjects)
		{
			for (size_t i = 0; i < mActiveObjects->size(); i++)
			{
				GameObject *obj = mActiveObjects->at(i);
				if (obj == mMainCharacter)
				{
					mGameTickPosition = i;
					break;
				}
			}
		}
	}

	const char *Game::getBaseDefinitionPath(int id) const
	{
		return "data/defs/";
	}

}
}
