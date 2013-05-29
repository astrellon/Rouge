#include "game_object.h"

#include "map.h"

#include "engine.h"
#include "tile_type.h"
#include "game.h"
#include "dialogue_component.h"
#include "loading_state.h"

#include <util/data_table.h>
#include <util/data_boolean.h>
#include <util/data_string.h>
#include <util/data_number.h>

namespace am {
namespace game {

	const int GameObject::LUA_ID = 0x08;
	const char *GameObject::LUA_TABLENAME = "am_game_GameObject";

	GameObject::GameObject() :
		Layer(),
		mLocationX(0.0f),
		mLocationY(0.0f),
		mCameraOffsetX(0.0f),
		mCameraOffsetY(0.0f),
		mFixedToGrid(false),
		mOnlyOnPassable(false),
		mMap(NULL),
		mOriginalMap(NULL)
	{
		setName("GameObject");
		Engine::getEngine()->registerGameObject(this);
	}
	GameObject::GameObject(const GameObject &copy) :
		Layer(copy),
		IAttributeData(copy),
		mName(copy.mName),
		mFixedToGrid(copy.mFixedToGrid),
		mOnlyOnPassable(copy.mOnlyOnPassable),
		mLocationX(copy.mLocationX),
		mLocationY(copy.mLocationY),
		mCameraOffsetX(copy.mCameraOffsetX),
		mCameraOffsetY(copy.mCameraOffsetY),
		mPassibleTypes(copy.mPassibleTypes),
		mMap(NULL),
		mOriginalMap(NULL)
	{
		if (copy.mMap)
		{
			copy.mMap->addGameObject(this);
			mMap = copy.mMap;
			mMap->retain();
			mOriginalMap = copy.mOriginalMap;
			if (mOriginalMap)
			{
				mOriginalMap->retain();
			}
		}
		
		if (copy.mDialogueComp)
		{
			mDialogueComp = new DialogueComponent(*copy.mDialogueComp);
			mDialogueComp->setAttachedTo(this);
		}
	}
	GameObject::~GameObject()
	{
		if (mMap)
		{
			Map *map = mMap;
			mMap = NULL;
			map->release();
		}
		if (mOriginalMap)
		{
			Map *map = mOriginalMap;
			mOriginalMap = NULL;
			map->release();
		}
	}

	void GameObject::update(float dt)
	{

	}

	void GameObject::setName(const char *name)
	{
		mName = name;
	}
	void GameObject::setName(const string &name)
	{
		mName = name;
	}
	string GameObject::getName() const
	{
		return mName;
	}

	void GameObject::setLocation(float x, float y, bool setDraw)
	{
		if (!mMap || (mMap && mMap->isValidLocation(x, y, this)))
		{
			mLocationX = x;
			mLocationY = y;
		}
		if (setDraw)
		{
			mTransform.setXY(x, y);
		}
		
	}
	float GameObject::getLocationX() const
	{
		return mLocationX;
	}
	float GameObject::getLocationY() const
	{
		return mLocationY;
	}

	bool GameObject::setGridLocation(int x, int y, bool setDraw)
	{
		float flocX = Engine::getEngine()->getGridXSize() * static_cast<float>(x);
		float flocY = Engine::getEngine()->getGridYSize() * static_cast<float>(y);
		int locX = static_cast<int>(flocX);
		int locY = static_cast<int>(flocY);
		if (!mMap || (mMap && mMap->isValidGridLocation(x, y, this)))
		{
			mLocationX = flocX;
			mLocationY = flocY;
			if (setDraw)
			{
				mTransform.setXY(mLocationX, mLocationY);
			}
			return true;
		}
		return false;
	}
	int GameObject::getGridLocationX() const
	{
		return static_cast<int>(mLocationX * Engine::getEngine()->getGridXSizeResp());
	}
	int GameObject::getGridLocationY() const
	{
		return static_cast<int>(mLocationY * Engine::getEngine()->getGridYSizeResp());
	}

	void GameObject::move(float x, float y)
	{
		const Engine *engine = Engine::getEngine();
		int gridX = static_cast<int>(mLocationX * engine->getGridXSizeResp());
		int gridY = static_cast<int>(mLocationY * engine->getGridYSizeResp());
		int newGridX = static_cast<int>((mLocationX + x) * engine->getGridXSizeResp());
		int newGridY = static_cast<int>((mLocationY + y) * engine->getGridYSizeResp());
		float dx = 0.0f;
		float dy = 0.0f;
		if (mMap->isValidGridLocation(gridX, newGridY, this))
		{
			dy = y;
		}
		if (mMap->isValidGridLocation(newGridX, gridY, this))
		{
			dx = x;
		}
		setLocation(mLocationX + dx, mLocationY + dy);
	}

	void GameObject::moveGrid(int x, int y)
	{
		const Engine *engine = Engine::getEngine();
		int gridX = static_cast<int>(mLocationX * engine->getGridXSizeResp());
		int gridY = static_cast<int>(mLocationY * engine->getGridYSizeResp());
		int newGridX = gridX + x;
		int newGridY = gridY + y;
		float dx = 0.0f;
		float dy = 0.0f;
		if (mMap->isValidGridLocation(gridX, newGridY, this))
		{
			dy = static_cast<float>(y) * engine->getGridYSize();
		}
		if (mMap->isValidGridLocation(newGridX, gridY, this))
		{
			dx = static_cast<float>(x) * engine->getGridXSize();
		}
		if (mMap->isValidGridLocation(newGridX, newGridY, this))
		{
			dy = static_cast<float>(y) * engine->getGridYSize();
			dx = static_cast<float>(x) * engine->getGridXSize();
		}
		else
		{
			dy = 0.0f;
			dx = 0.0f;
		}
		setLocation(mLocationX + dx, mLocationY + dy);
	}

	void GameObject::setCameraOffset(float x, float y)
	{
		mCameraOffsetX = x;
		mCameraOffsetY = y;
	}
	float GameObject::getCameraOffsetX() const
	{
		return mCameraOffsetX;
	}
	float GameObject::getCameraOffsetY() const
	{
		return mCameraOffsetY;
	}

	void GameObject::setMap(Map *map)
	{
		if (mMap)
		{
			mMap->release();
		}
		mMap = map;
		// First map we were added to is our original map.
		if (mOriginalMap == NULL)
		{
			setOriginalMap(map);
		}
		if (mMap)
		{
			mMap->retain();
		}
	}
	Map *GameObject::getMap() const
	{
		return mMap;
	}

	void GameObject::setOriginalMap(Map *map)
	{
		if (mOriginalMap)
		{
			mOriginalMap->release();
		}
		mOriginalMap = map;
		if (mOriginalMap)
		{
			mOriginalMap->retain();
		}
	}
	Map *GameObject::getOriginalMap() const
	{
		return mOriginalMap;
	}
	void GameObject::setFixedToGrid(bool fixed)
	{
		mFixedToGrid = fixed;
	}
	bool GameObject::isFixedToGrid() const
	{
		return mFixedToGrid;
	}

	void GameObject::setOnlyOnPassable(bool only)
	{
		mOnlyOnPassable = only;
	}
	bool GameObject::isOnlyOnPassable() const
	{
		return mOnlyOnPassable;
	}

	void GameObject::addPassibleType(TileType *tileType)
	{
		if (tileType != NULL)
		{
			mPassibleTypes.push_back(tileType);
		}
	}
	void GameObject::removePassibleType(TileType *tileType)
	{
		if (tileType)
		{
			return;
		}
		for (size_t i = 0; i < mPassibleTypes.size(); i++)
		{
			if (mPassibleTypes[i] == tileType)
			{
				mPassibleTypes.erase(mPassibleTypes.begin() + i);
			}
		}
	}
	void GameObject::removeAllPassibleTypes()
	{
		mPassibleTypes.clear();
	}
	bool GameObject::hasPassibleType(TileType *tileType) const
	{
		if (tileType == NULL)
		{
			return false;
		}
		for (size_t i = 0; i < mPassibleTypes.size(); i++)
		{
			if (mPassibleTypes[i] == tileType)
			{
				return true;
			}
		}
		return false;
	}
	GameObject::PassibleTypeList &GameObject::getPassibleTypes()
	{
		return mPassibleTypes;
	}
	const GameObject::PassibleTypeList &GameObject::getPassibleTypes() const
	{
		return mPassibleTypes;
	}

	/*void GameObject::talkTo(GameObject *other)
	{
		if (mDialogueComp.get() != NULL)
		{
			mDialogueComp->talkTo(other);
		}
		//mTalkingTo = other;
		//Handle<DialogueEvent> e(new DialogueEvent(other->getStartDialogue()));
		//fireEvent<DialogueEvent>(e);
	}
	void GameObject::talkTo(GameObject *other, Dialogue *diag)
	{
		if (mDialogueComp.get() != NULL)
		{
			mDialogueComp->talkTo(other, diag);
		}
		//mTalkingTo = other;
		//Handle<DialogueEvent> e(new DialogueEvent(diag));
		//fireEvent<DialogueEvent>(e);
	}*/
	/*GameObject *GameObject::getTalkingTo() const
	{
		return mTalkingTo;
	}*/

	bool GameObject::setGameId(const char *id)
	{
		Game *game = Engine::getEngine()->getCurrentGame();
		if (!game)
		{
			return false;
		}
		GameObject *other = game->getGameObject(id);
		if (other == this)
		{
			return true;
		}
		if (other != NULL)
		{
			return false;
		}
		game->deregisterGameObject(this);
		mGameId = id;
		game->registerGameObject(this);
		return true;
	}
	const char *GameObject::getGameId() const
	{
		return mGameId.c_str();
	}

	void GameObject::setDialogueComp(DialogueComponent *comp, bool setAttached)
	{
		if (setAttached && comp)
		{
			comp->setAttachedTo(this);
		}
		mDialogueComp = comp;
	}
	DialogueComponent *GameObject::getDialogueComp() const
	{
		return mDialogueComp;
	}

	Source *GameObject::getSource(bool create)
	{
		if (create && !mSoundSource)
		{
			setSource(new Source());
		}
		return mSoundSource;
	}
	void GameObject::setSource(Source *source)
	{
		mSoundSource = source;
		applyToSource();
	}

	data::IData *GameObject::serialise()
	{
		data::Table *output = new data::Table();
		output->at("gameId", mGameId);
		output->at("fixedToGrid", mFixedToGrid);
		output->at("onlyOnPassable", mOnlyOnPassable);
		output->at("locationX", mLocationX);
		output->at("locationY", mLocationY);
		output->at("cameraOffsetX", mCameraOffsetX);
		output->at("cameraOffsetY", mCameraOffsetY);
		output->at("name", mName);

		Handle<data::Table> passibleTypes(new data::Table());
		for (auto iter = mPassibleTypes.begin(); iter != mPassibleTypes.end(); ++iter)
		{
			passibleTypes->push((*iter)->getName());
		}
		output->at("passibleTypes", passibleTypes);
		if (mMap)
		{
			output->at("map", mMap->getFilename());
		}
		if (mOriginalMap)
		{
			output->at("originalMap", mOriginalMap->getFilename());
		}

		if (mDialogueComp)
		{
			output->at("dialogueComponent", mDialogueComp->serialise());
		}

		IAttributeData::serialise(output);
		return output;
	}

	int GameObject::deserialise(LoadingState *state, data::IData *data)
	{
		if (!state || !data)
		{
			am_log("LOADERR", "Loading game object as loading state or data was NULL");
			return 0;
		}
		
		Handle<data::Table> dataMap(data::Table::checkDataType(data, "game object"));
		if (!dataMap)
		{
			return -1;
		}

		Handle<data::String> str(dataMap->at<data::String>("gameId"));
		if (str)
		{
			state->setGameId(str->string(), this);
		}
		
		Handle<data::Boolean> boo(dataMap->at<data::Boolean>("fixedToGrid"));
		if (boo)
		{
			mFixedToGrid = boo->boolean();
		}

		boo = dataMap->at<data::Boolean>("onlyOnPassable");
		if (boo)
		{
			mOnlyOnPassable = boo->boolean();
		}

		float locationX = mLocationX;
		float locationY = mLocationY;
		Handle<data::Number> num(dataMap->at<data::Number>("locationX"));
		if (num)
		{
			locationX = num->number<float>();
		}
		num = dataMap->at<data::Number>("locationY");
		if (num)
		{
			locationY = num->number<float>();
		}
		setLocation(locationX, locationY);

		num = dataMap->at<data::Number>("cameraOffsetX");
		if (num)
		{
			mCameraOffsetX = num->number<float>();
		}
		num = dataMap->at<data::Number>("cameraOffsetY");
		if (num)
		{
			mCameraOffsetY = num->number<float>();
		}

		str = dataMap->at<data::String>("name");
		if (str)
		{
			setName(str->string());
		}

		Handle<data::Table> arr(dataMap->at<data::Table>("passibleTypes"));
		if (arr)
		{
			Engine *engine = Engine::getEngine();
			for (auto iter = arr->beginArray(); iter != arr->endArray(); ++iter)
			{
				const char *tileTypeName = (*iter)->string();
				if (!tileTypeName)
				{
					stringstream ss;
					ss << "Unable to set tile type from type '" << (*iter)->typeName();
					ss << "' must be a String.";
					am_log("LOADERR", ss);
					continue;
				}
				TileType *tileType = engine->getTileType(tileTypeName);
				if (!tileType)
				{
					stringstream ss;
					ss << "Unable to find tile type '" << tileTypeName << "'.";
					am_log("LOADERR", ss);
					continue;
				}
				addPassibleType(tileType);
			}
		}

		str = dataMap->at<data::String>("map");
		if (str)
		{
			const char *mapName = str->string();
			if (!mapName)
			{
				am_log("LOADERR", "Unable to add game object to NULL or empty string map.");
			}
			else
			{
				state->addGameObjectToMap(this, mapName);
			}
		}

		str = dataMap->at<data::String>("originalMap");
		if (str)
		{
			state->addMapToLoad(str->string());
		}

		Handle<data::IData> tempData(dataMap->at("dialogueComponent"));
		if (tempData)
		{
			DialogueComponent *comp = new DialogueComponent();
			comp->deserialise(state, tempData);
			setDialogueComp(comp);
		}

		IAttributeData::deserialise(state, dataMap);

		return 1;
	}

	void GameObject::applyToSource()
	{
		if (!mSoundSource)
		{
			return;
		}
		mSoundSource->setPosition(mLocationX, mLocationY);
	}

	const char *GameObject::getGameObjectTypeName() const
	{
		return "gameobject";
	}
}
}
