#include "game.h"

#include <log/logger.h>

#include <sstream>
#include <stdio.h>

#include <gfx/gfx_layer.h>
#include <gfx/gfx_engine.h>

#include <ui/mouse_manager.h>
#include <ui/ui_game_hud.h>
#include <ui/ui_inspector.h>
#include <ui/keyboard_manager.h>

#include "engine.h"
#include "map.h"
#include "player_hand.h"
#include "character.h"

namespace am {
namespace game {

	//Game::DialogueMap Game::mDialogueMap;

	Game::Game(Engine *engine) :
		mEngine(engine),
		mCurrentMap(NULL),
		mActiveObjects(NULL)
	{
		if (engine == NULL)
		{
			mEngine = Engine::getEngine();
		}
		mGameLayer = new Layer();
		mGameLayer->setName("Game->GameLayer");
		mGameLayer->setInteractive(true);

		mBackground = new Layer();
		mBackground->setName("Background");
		mBackground->setInteractive(true);
		mGameLayer->addChild(mBackground.get());

		mItemLayer = new Layer();
		mItemLayer->setName("ItemLayer");
		mItemLayer->setInteractive(true);
		mGameLayer->addChild(mItemLayer.get());

		mCharacterLayer = new Layer();
		mCharacterLayer->setName("CharacterLayer");
		mCharacterLayer->setInteractive(true);
		mGameLayer->addChild(mCharacterLayer.get());

		mForeground = new Layer();
		mForeground->setName("Foreground");
		mForeground->setInteractive(false);
		mGameLayer->addChild(mForeground.get());
	}
	Game::~Game()
	{
		deinit();
	}

	void Game::deinit()
	{
		mCamera.followObject(NULL);
		mGameLayer->deinit();
		{
			DialogueMap dialogue = mDialogueMap;
			DialogueMap::iterator iter;
			for (iter = dialogue.begin(); iter != dialogue.end(); ++iter)
			{
				delete iter->second;
			}
			mDialogueMap.clear();
		}
		{
			MapMap maps = mMaps;
			MapMap::iterator iter;
			for (iter = maps.begin(); iter != maps.end(); ++iter)
			{
				iter->second->deinit();
			}
			mMaps.clear();
		}
		/*mGameObjects.clear();
		Handle<IController> cont(mMainCharacter->getController());
		if (cont)
		{
			cont->detach();
			mMainCharacter->setController(NULL);
		}*/
	}

	Map *Game::getMapLua(const char *mapName)
	{
		return getMapLua(string(mapName));
	}
	Map *Game::getMapLua(const string &mapName)
	{
		MapMap::iterator iter = mMaps.find(mapName);
		if (iter != mMaps.end())
		{
			return iter->second.get();
		}

		stringstream ss;
		ss << "data/maps/" << mapName << ".lua";
		LuaState lua(false);
		if (!lua.loadFile(ss.str().c_str()))
		{
			stringstream errss;
			errss << "Unable to load map '" << mapName << "', using the path '";
			errss << ss.str() << '\''; 
			am_log("MAP", errss);
			lua.logStack("MAPLUA");
			lua.close();
			return NULL;
		}
		Handle<Map> map(new Map(mapName.c_str()));
		lua_getglobal(lua, "map");
		map->loadDef(lua);

		mMaps[mapName] = map;
		lua.close();
		return map.get();
	}

	Map *Game::getCurrentMap()
	{
		return mCurrentMap.get();
	}

	void Game::onEvent(MouseEvent *e)
	{
		if (!mCurrentMap.get() || e->getTarget() != mCurrentMap)
		{
			return;
		}

		float localX = static_cast<float>(e->getLocalMouseX());
		float localY = static_cast<float>(e->getLocalMouseY());
		
		if (localX < 0 || localY < 0 ||
			localX > mCurrentMap->getWidth() ||
			localY > mCurrentMap->getHeight())
		{
			return;
		}
		if (e->getMouseButton() == LEFT_BUTTON)
		{
			PlayerHand *hand = PlayerHand::getPlayerHand();
			if (hand && hand->getInhand() != NULL)
			{
				if (mMainCharacter->dropItem(hand->getInhand(), localX, localY))
				{
					mItemLayer->addChild(hand->getInhand());
					hand->setInhand(NULL);
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

		if (clickedOn.size() > 0 && e->getMouseButton() == LEFT_BUTTON)
		{
			Item *item = dynamic_cast<Item *>(clickedOn[0].get());
			if (item)
			{
				// If holding shift, it goes straight into the inventory.
				if (KeyboardManager::getManager()->isKeyDown(16))
				{
					mMainCharacter->pickupItem(item);
				}
				else
				{
					PlayerHand *hand = PlayerHand::getPlayerHand();
					if (hand && hand->getInhand() == NULL)
					{
						hand->setInhand(item);
						mItemLayer->removeChild(item);
						item->setItemLocation(Item::HAND);
					}
				}
				return;
			}
			GameObject *obj = dynamic_cast<GameObject *>(clickedOn[0].get());
			if (mMainCharacter->getDialogueComp() && obj && obj->getDialogueComp() && obj->getDialogueComp()->getStartDialogue())
			{
				mMainCharacter->getDialogueComp()->talkTo(obj);
				return;
			}
		}
		else
		{
			GameHud *gameHud = Engine::getEngine()->getGameHud();
			if (gameHud)
			{
				int gridX = static_cast<int>(localX * Engine::getEngine()->getGridXSizeResp());
				int gridY = static_cast<int>(localY * Engine::getEngine()->getGridYSizeResp());

				Tile *tile = mCurrentMap->getTile(gridX, gridY);
				Inspector *inspector = gameHud->getInspector();
				inspector->setTile(tile);

				inspector->clearGameObjects();
				inspector->addGameObjects(clickedOn);
			}
		}
	}

	bool Game::addMap(Map *map)
	{
		if (map && map->getName().size() > 0)
		{
			mMaps[map->getName()] = map;
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
			string mapStr(mapName);
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
	
	void Game::setCurrentMap(Map *map, bool addMap)
	{
		mBackground->clear();
		mItemLayer->clear();
		mCharacterLayer->clear();
		mForeground->clear();
		mCamera.followObject(NULL);

		if (mCurrentMap)
		{
			mCurrentMap->removeEventListener(MOUSE_UP, this);
		}

		mCurrentMap = map;
		if (map)
		{
			mBackground->addChild(map->getBackground());
			mForeground->addChild(map->getForeground());
			mActiveObjects = map->getObjects();

			mCurrentMap->addEventListener(MOUSE_UP, this);
			
			if (mActiveObjects)
			{
				ObjectList::iterator iter;
				for (iter = mActiveObjects->begin(); iter != mActiveObjects->end(); ++iter)
				{
					mCharacterLayer->addChild(iter->get());
				}
			}
			else
			{
				stringstream errss;
				errss << "Map (" << map->getName() << ") return a NULL object list.";
				am_log("MAP", errss);
			}
			if (addMap && map->getName().size() > 0)
			{
				mMaps[map->getName()] = map;
			}
		}
	}
	void Game::setCurrentMap(const char *mapName)
	{
		setCurrentMap(getMapLua(mapName));
	}
	void Game::setCurrentMap(const string &mapName)
	{
		setCurrentMap(getMapLua(mapName));
	}

	bool Game::addGameObjectToMap(GameObject *object)
	{
		if (mCurrentMap.get() == NULL)
		{
			am_log("MAP", "Unable to add game object to null current map");
			return false;
		}
		if (mCurrentMap->addGameObject(object))
		{
			if (strcmp(object->getGameObjectTypeName(), "character") == 0)
			{
				mCharacterLayer->addChild(object);
			}
			else
			{
				mItemLayer->addChild(object);
			}
			return true;
		}
		return false;
	}
	bool Game::removeGameObjectFromMap(GameObject *object)
	{
		if (mCurrentMap.get() == NULL)
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
		if (mCurrentMap.get() == NULL)
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
				mCharacterLayer->addChild(object);
			}
		}
	}
	void Game::moveObjectToMapGrid(GameObject *object, const char *mapName, int x, int y, bool setAsCurrent)
	{
		float gx = Engine::getEngine()->getGridXSize() * static_cast<float>(x);
		float gy = Engine::getEngine()->getGridYSize() * static_cast<float>(y);
		moveObjectToMap(object, mapName, gx, gy, setAsCurrent);
	}
	void Game::moveObjectToMapGrid(GameObject *object, Map *map, int x, int y, bool setAsCurrent)
	{
		float gx = Engine::getEngine()->getGridXSize() * static_cast<float>(x);
		float gy = Engine::getEngine()->getGridYSize() * static_cast<float>(y);
		moveObjectToMap(object, map, gx, gy, setAsCurrent);
	}

	Camera *Game::getCamera()
	{
		return &mCamera;
	}

	Layer *Game::getGameLayer()
	{
		return mGameLayer.get();
	}
	Layer *Game::getBackground()
	{
		return mBackground.get();
	}
	Layer *Game::getItemLayer()
	{
		return mItemLayer.get();
	}
	Layer *Game::getCharacterLayer()
	{
		return mCharacterLayer.get();
	}
	Layer *Game::getForeground()
	{
		return mForeground.get();
	}

	void Game::setMainCharacter(Character *character)
	{
		mMainCharacter = character;
	}
	Character *Game::getMainCharacter()
	{
		return mMainCharacter;
	}

	void Game::update(float dt)
	{
		if (mActiveObjects)
		{
			ObjectList::iterator iter;
			for (iter = mActiveObjects->begin(); iter != mActiveObjects->end(); ++iter)
			{
				iter->get()->update(dt);
			}
		}
		mCamera.update(dt);
	}

	GameObject *Game::getGameObject(const char *id) const
	{
		if (id == NULL || id[0] == '\0')
		{
			return NULL;
		}
		GameObjectIdMap::const_iterator iter = mGameObjects.find(string(id));
		if (iter != mGameObjects.end())
		{
			return iter->second;
		}
		return NULL;
	}
	void Game::registerGameObject(GameObject *obj)
	{
		if (obj == NULL)
		{
			return;
		}
		mGameObjects[obj->getGameId()] = obj;
	}
	void Game::deregisterGameObject(const char *id)
	{
		if (id && id[0] != '\0')
		{
			string idStr(id);
			GameObjectIdMap::iterator iter = mGameObjects.find(idStr);
			if (iter != mGameObjects.end())
			{
				mGameObjects.erase(iter);
			}
		}
	}
	void Game::deregisterGameObject(GameObject *obj)
	{
		if (obj == NULL)
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
			string id = dialogue->getId();
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
		if (id != NULL)
		{
			DialogueMap::const_iterator iter = mDialogueMap.find(string(id));
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
		if (id != NULL)
		{
			DialogueMap::iterator iter = mDialogueMap.find(string(id));
			if (iter != mDialogueMap.end())
			{
				return iter->second;
			}
		}
		return NULL;
	}

	void Game::getAvailableDialogues(vector<Dialogue *> &result, const GameObject *talker, const GameObject *talkedTo)
	{
		if (talker == NULL || talkedTo == NULL || 
			talker->getDialogueComp() == NULL || talkedTo->getDialogueComp() == NULL)
		{
			return;
		}

		const Character::SubjectMap &unlocked = talker->getDialogueComp()->getUnlockedSubjects();
		const Character::SubjectMap &available = talkedTo->getDialogueComp()->getDialoguesAvailable();
		Character::SubjectMap::const_iterator iter;
		for (iter = available.begin(); iter != available.end(); ++iter)
		{
			Dialogue *dialogue = getDialogue(iter->first.c_str());
			if (dialogue == NULL)
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
				Character::SubjectMap::const_iterator iter = unlocked.find(string(dialogue->getSubject()));
				if (iter != unlocked.end() && !iter->second)
				{
					result.push_back(dialogue);
				}
			}
		}
	}

	bool Game::addQuest(Quest *quest)
	{
		if (quest == NULL)
		{
			return false;
		}
		QuestMap::const_iterator iter = mQuestMap.find(quest->getQuestId());
		if (iter == mQuestMap.end())
		{
			mQuestMap[quest->getQuestId()] = quest;
			return true;
		}
		return  false;
	}
	bool Game::removeQuest(const char *questId)
	{
		if (questId == NULL || questId[0] == '\0')
		{
			return false;
		}
		QuestMap::const_iterator iter = mQuestMap.find(string(questId));
		if (iter != mQuestMap.end())
		{
			mQuestMap.erase(iter);
			return true;
		}
		return  false;
	}
	Quest *Game::getQuest(const char *questId)
	{
		if (questId == NULL || questId[0] == '\0')
		{
			return NULL;
		}
		QuestMap::const_iterator iter = mQuestMap.find(string(questId));
		if (iter != mQuestMap.end())
		{
			return iter->second;
		}
		return NULL;
	}

}
}
