#include "game_object.h"

#include <gl.h>

#include "map.h"

#include "engine.h"
#include "tile_type.h"
#include "game.h"
#include "dialogue_component.h"
#include "loading_state.h"

#include <math/math.h>

#include <gfx/gfx_tooltip_string.h>

#include <util/data_table.h>
#include <util/data_boolean.h>
#include <util/data_string.h>
#include <util/data_number.h>

#include <sfx/sfx_source_point.h>
#include <sfx/sfx_source_area.h>

#include <lua/wrappers/lua_id_table.h>
#include <lua/wrappers/game/lua_game.h>

namespace am {
namespace game {

	const int GameObject::LUA_ID = LUA_ID_GAMEOBJECT;
	const char *GameObject::LUA_TABLENAME = LUA_TABLE_GAMEOBJECT;
	const char *GameObject::sDamageTypeNames[] = 
	{
		"unknown", "arrow", "blunt", "piercing", "falling", "fire", "cold", "magical", "acid", "MAX_DAMAGE_LENGTH"
	};

	GameObject::GameObject() :
		gfx::Layer(),
		mLocationX(0.0f),
		mLocationY(0.0f),
		mCameraOffsetX(0.0f),
		mCameraOffsetY(0.0f),
		mFixedToGrid(false),
		mOnlyOnPassable(false),
		mMap(nullptr),
		mOriginalMap(nullptr),
        mInteractWithFunc(LUA_REFNIL)
	{
		setName("GameObject");
		Engine::getEngine()->registerGameObject(this);
		setInteractive(true);
	}
	GameObject::GameObject(const GameObject &copy) :
		gfx::Layer(copy),
		IAttributeData(copy),
		mName(copy.mName),
		mFixedToGrid(copy.mFixedToGrid),
		mOnlyOnPassable(copy.mOnlyOnPassable),
		mLocationX(copy.mLocationX),
		mLocationY(copy.mLocationY),
		mCameraOffsetX(copy.mCameraOffsetX),
		mCameraOffsetY(copy.mCameraOffsetY),
		mPassibleTypes(copy.mPassibleTypes),
		mMap(nullptr),
		mOriginalMap(nullptr),
		mDescription(copy.mDescription),
        mInteractWithFunc(copy.mInteractWithFunc)
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

		setInteractive(copy.isInteractive());
	}
	GameObject::~GameObject()
	{
		if (mMap)
		{
			Map *map = mMap;
			mMap = nullptr;
			map->release();
		}
		if (mOriginalMap)
		{
			Map *map = mOriginalMap;
			mOriginalMap = nullptr;
			map->release();
		}
        if (mInteractWithFunc != LUA_REFNIL)
        {
            luaL_unref(Engine::getEngine()->getLua(), LUA_REGISTRYINDEX, mInteractWithFunc);
            mInteractWithFunc = LUA_REFNIL;
        }
	}

	void GameObject::update(float dt)
	{

	}

	bool GameObject::onGameTick(float dt)
	{
		return true;
	}

	void GameObject::setName(const char *name)
	{
		if (name)
		{
			mName = name;
			//setTooltip(new TooltipString(name, nullptr));
		}
	}
	void GameObject::setName(const string &name)
	{
		mName = name;
	}
	std::string GameObject::getName() const
	{
		return mName;
	}

	void GameObject::setDescription(const char *description)
	{
		if (description)
		{
			mDescription = description;
		}
	}
	void GameObject::setDescription(const string &description)
	{
		mDescription = description;
	}
	std::string GameObject::getDescription() const
	{
		return mDescription;
	}

	void GameObject::setLocation(float x, float y, bool setDraw)
	{
		if (!mMap || (mMap && mMap->isValidLocation(x, y, this)))
		{
			mLocationX = x;
			mLocationY = y;
		}
		if (mSoundSource)
		{
			mSoundSource->setPosition(x, y);
		}
		if (setDraw)
		{
			if (mFixedToGrid)
			{
				Engine *engine = Engine::getEngine();
				x = math::round(x * engine->getGridSizeResp()) * engine->getGridSize();
				y = math::round(y * engine->getGridSizeResp()) * engine->getGridSize();
			}
			mTransform.setXY(x, y);
		}
		if (mMap)
		{
			mMap->checkMapRegion(this);
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
		float flocX = Engine::getEngine()->getGridSize() * static_cast<float>(x);
		float flocY = Engine::getEngine()->getGridSize() * static_cast<float>(y);
		int locX = static_cast<int>(flocX);
		int locY = static_cast<int>(flocY);
		if (!mMap || (mMap && mMap->isValidGridLocation(x, y, this)))
		{
			mLocationX = flocX;
			mLocationY = flocY;
			if (mMap)
			{
				mMap->checkMapRegion(this);
			}
			if (mSoundSource)
			{
				mSoundSource->setPosition(mLocationX, mLocationY);
			}
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
		return math::round(mLocationX * Engine::getEngine()->getGridSizeResp());
	}
	int GameObject::getGridLocationY() const
	{
		return math::round(mLocationY * Engine::getEngine()->getGridSizeResp());
	}

	bool GameObject::setGridLocationF(float x, float y, bool setDraw)
	{
		float flocX = Engine::getEngine()->getGridSize() * x;
		float flocY = Engine::getEngine()->getGridSize() * y;
		int locX = static_cast<int>(flocX);
		int locY = static_cast<int>(flocY);
		if (!mMap || (mMap && mMap->isValidLocation(flocX, flocY, this)))
		{
			mLocationX = flocX;
			mLocationY = flocY;
			if (mSoundSource)
			{
				mSoundSource->setPosition(mLocationX, mLocationY);
			}
			if (setDraw)
			{
				mTransform.setXY(mLocationX, mLocationY);
			}
			return true;
		}
		return false;
	}
	float GameObject::getGridLocationXF() const
	{
		return mLocationX * Engine::getEngine()->getGridSizeResp();
	}
	float GameObject::getGridLocationYF() const
	{
		return mLocationY * Engine::getEngine()->getGridSizeResp();
	}

	float GameObject::distanceTo(GameObject *obj) const
	{
		if (!obj)
		{
			return 0.0f;
		}
		float dx = obj->mLocationX - mLocationX;
		float dy = obj->mLocationY - mLocationY;
		return sqrt(dx * dx + dy * dy);
	}
	float GameObject::distanceToGrid(GameObject *obj) const
	{
		if (!obj)
		{
			return 0.0f;
		}
		int dx = obj->getGridLocationX() - getGridLocationX();
		int dy = obj->getGridLocationY() - getGridLocationY();
		return sqrt(static_cast<float>(dx * dx + dy * dy));
	}

    base::ReturnCode GameObject::interactWith(GameObject *interacter, bool byMovement)
	{
        int funcRef = getInteractWithFunc();
        if (funcRef != LUA_REFNIL)
        {
            // Call lua function.
            lua::LuaState &lua = Engine::getEngine()->getLua();
            lua_rawgeti(lua, LUA_REGISTRYINDEX, getInteractWithFunc());
            lua::game::wrapGameObject(lua, this);
            lua::game::wrapGameObject(lua, interacter);
            lua_pushboolean(lua, byMovement);
            lua_acall(lua, 3, 1);

            int result = lua_tointeger(lua, -1);
            if (result == base::DID_INTERACT || result == base::DO_NOT_INTERACT)
            {
                return static_cast<base::ReturnCode>(result);
            }
        }
		return base::DID_NOT_INTERACT;
	}
    base::ReturnCode GameObject::interactDialogue(GameObject *interacter, bool byMovement)
    {
        if (interacter->getGameObjectType() != CHARACTER)
		{
			return base::DID_NOT_INTERACT;
		}
		Character *obj = dynamic_cast<Character *>(interacter);
		// Don't interact with yourself and currently only interact with the main character.
		if (!obj || obj == this || obj != Engine::getGame()->getMainCharacter())
		{
			return base::DO_NOT_INTERACT;
		}
		if (obj->getDialogueComp() && getDialogueComp() && getDialogueComp()->getStartDialogue())
		{
			obj->getDialogueComp()->talkTo(this);
		}
		return base::DID_INTERACT;
    }
	bool GameObject::interactWithLayer() const
	{
		return true;
	}

	void GameObject::addToMapRegion(MapRegion *region)
	{
		if (!region || isInMapRegion(region))
		{
			return;
		}
		region->gameObjectEntered(this);
		mMapRegions.push_back(region);
	}
	void GameObject::removeFromMapRegion(MapRegion *region)
	{
		if (!region)
		{
			return;
		}
		for (auto iter = mMapRegions.begin(); iter != mMapRegions.end(); ++iter)
		{
			MapRegion *iterRegion = iter->get();
			if (iterRegion == region)
			{
				iterRegion->gameObjectExited(this);
				mMapRegions.erase(iter);
				break;
			}
		}
	}
	bool GameObject::isInMapRegion(MapRegion *region) const
	{
		if (!region)
		{
			return false;
		}
		for (auto iter = mMapRegions.begin(); iter != mMapRegions.end(); ++iter)
		{
			if (iter->get() == region)
			{
				return true;
			}
		}
		return false;
	}
	const MapRegion::MapRegionList &GameObject::getMapRegions() const
	{
		return mMapRegions;
	}

	bool GameObject::move(float x, float y)
	{
		const Engine *engine = Engine::getEngine();
		int gridX = static_cast<int>(mLocationX * engine->getGridSizeResp());
		int gridY = static_cast<int>(mLocationY * engine->getGridSizeResp());
		int newGridX = static_cast<int>((mLocationX + x) * engine->getGridSizeResp());
		int newGridY = static_cast<int>((mLocationY + y) * engine->getGridSizeResp());
 		ObjectList objs;
		if (mMap->getGameObjectsAt(newGridX, newGridY, objs))
		{
			if (objs.size() > 0)
			{
                base::ReturnCode interacted = objs[0]->interactWith(this, true);
				if (interacted != base::DID_NOT_INTERACT)
				{
					return false;
				}
			}
		}
		float dx = 0.0f;
		float dy = 0.0f;
		int valid = 0;
		if (mMap->isValidGridLocation(gridX, newGridY, this))
		{
			valid++;
			dy = y;
		}
		if (mMap->isValidGridLocation(newGridX, gridY, this))
		{
			valid++;
			dx = x;
		}
		if (valid > 0)
		{
			float newPosX = mLocationX + x;
			float newPosY = mLocationY + y;
			if (!mMap->isValidLocation(newPosX, newPosY, this))
			{
				return false;
			}
			setLocation(mLocationX + dx, mLocationY + dy);
		}
		return valid > 0;
	}

	bool GameObject::moveGrid(int x, int y)
	{
		const Engine *engine = Engine::getEngine();
		int gridX = static_cast<int>(mLocationX * engine->getGridSizeResp());
		int gridY = static_cast<int>(mLocationY * engine->getGridSizeResp());
		int newGridX = gridX + x;
		int newGridY = gridY + y;
		ObjectList objs;
		if (mMap->getGameObjectsAt(newGridX, newGridY, objs))
		{
			//if (objs.size() > 0 && objs[0]->interactWith(this, true))
			if (objs.size() > 0)
			{
                base::ReturnCode interacted = objs[0]->interactWith(this, true);
				if (interacted != base::DID_NOT_INTERACT)
				{
					return false;
				}
			}
		}
		float dx = 0.0f;
		float dy = 0.0f;
		int valid = 0;
		if (mMap->isValidGridLocation(gridX, newGridY, this))
		{
			valid++;
			dy = static_cast<float>(y) * engine->getGridSize();
		}
		if (mMap->isValidGridLocation(newGridX, gridY, this))
		{
			valid++;
			dx = static_cast<float>(x) * engine->getGridSize();
		}
		if (mMap->isValidGridLocation(newGridX, newGridY, this))
		{
			valid++;
			dy = static_cast<float>(y) * engine->getGridSize();
			dx = static_cast<float>(x) * engine->getGridSize();
		}
		else
		{
			dy = 0.0f;
			dx = 0.0f;
		}
		if (valid > 0)
		{
			setLocation(mLocationX + dx, mLocationY + dy);
		}
		return valid > 0;
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
		if (mOriginalMap == nullptr)
		{
			setOriginalMap(map);
		}
		if (mMap)
		{
			mMap->retain();
			mMap->checkMapRegion(this);
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

	void GameObject::receiveDamage(float damage)
	{
		// TODO
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
		if (tileType != nullptr)
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
			if (mPassibleTypes[i].get() == tileType)
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
		if (tileType == nullptr)
		{
			return false;
		}
		for (size_t i = 0; i < mPassibleTypes.size(); i++)
		{
			if (mPassibleTypes[i].get() == tileType)
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
		if (other != nullptr)
		{
			return false;
		}
		registerSelf(id);
		return true;
	}
	const char *GameObject::getGameId() const
	{
		return mGameId.c_str();
	}
	void GameObject::registerSelf(const char *id)
	{
		Game *game = Engine::getEngine()->getCurrentGame();
		if (!game)
		{
			return;
		}
		game->deregisterGameObject(this);
		mGameId = id;
		game->registerGameObject(this);
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

	sfx::ISource *GameObject::getSource(bool create)
	{
		if (create && !mSoundSource)
		{
			setSource(new sfx::SourcePoint());
		}
		return mSoundSource;
	}
	void GameObject::setSource(sfx::ISource *source)
	{
		mSoundSource = source;
		applyToSource();
	}

    void GameObject::setInteractWithFunc(int funcRef)
    {
        mInteractWithFunc = funcRef;
    }
    int GameObject::getInteractWithFunc() const
    {
        return mInteractWithFunc;
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

		base::Handle<data::Table> passibleTypes(new data::Table());
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
			am_log("LOADERR", "Loading game object as loading state or data was nullptr");
			return 0;
		}
		
		base::Handle<data::Table> dataMap(data::Table::checkDataType(data, "game object"));
		if (!dataMap)
		{
			return -1;
		}

		base::Handle<data::String> str(dataMap->at<data::String>("gameId"));
		if (str)
		{
			state->setGameId(str->string(), this);
			mGameId = str->string();
		}
		
		base::Handle<data::Boolean> boo(dataMap->at<data::Boolean>("fixedToGrid"));
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
		base::Handle<data::Number> num(dataMap->at<data::Number>("locationX"));
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

		base::Handle<data::Table> arr(dataMap->at<data::Table>("passibleTypes"));
		if (arr)
		{
			Engine *engine = Engine::getEngine();
			for (auto iter = arr->beginArray(); iter != arr->endArray(); ++iter)
			{
				const char *tileTypeName = (*iter)->string();
				if (!tileTypeName)
				{
					std::stringstream ss;
					ss << "Unable to set tile type from type '" << (*iter)->typeName();
					ss << "' must be a String.";
					am_log("LOADERR", ss);
					continue;
				}
				base::Handle<TileType> tileType(engine->getTileType(tileTypeName));
				if (!tileType)
				{
					std::stringstream ss;
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
				am_log("LOADERR", "Unable to add game object to nullptr or empty string map.");
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

		base::Handle<data::IData> tempData(dataMap->at("dialogueComponent"));
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
		std::stringstream ss;
		ss << "Setting sound source position: " << mLocationX << ", " << mLocationY;
		am_log("SFX", ss);
		mSoundSource->setPosition(mLocationX, mLocationY);
	}

	const char *GameObject::getGameObjectTypeName() const
	{
		return "gameobject";
	}
	GameObject::GameObjectType GameObject::getGameObjectType() const
	{
		return GAME_OBJECT;
	}

	void GameObject::postRender(float dt)
	{
		if (mSoundSource)
		{
			sfx::SourceArea *area = dynamic_cast<sfx::SourceArea *>(mSoundSource.get());
			if (area)
			{
				math::Vector2f rel = area->getPosition().sub(mLocationX, mLocationY);
				glBegin(GL_LINES);
				glColor3f(1.0f, 0.0f, 0.0f);
				glPushMatrix();
				glTranslatef(rel.x, rel.y, 0.0f);
				glVertex2f(0.0f, 0.0f);
				glVertex2f(area->getWidth(), 0.0f);
				glVertex2f(area->getWidth(), area->getHeight());
				glVertex2f(0.0f, area->getHeight());
				glPopMatrix();
				glEnd();

				math::Vector2f relPos = area->getClosestPosition().sub(mLocationX, mLocationY);
				glBegin(GL_TRIANGLES);
				//glPushMatrix();
				//glTranslatef(relPos.x, relPos.y, 0.0f);
				glColor3f(0.2f, 1.0f, 0.2f);
				glVertex2f(relPos.x - 5.0f, relPos.y - 5.0f);
				glVertex2f(relPos.x, relPos.y + 5.0f);
				glVertex2f(relPos.x + 5.0f, relPos.y - 5.0f);
				//glPopMatrix();
				glEnd();
			}
		}
		Layer::postRender(dt);
	}

	const char *GameObject::getDamageTypeName(DamageType type)
	{
		if (type < 0 || type >= MAX_DAMAGE_TYPE)
		{
			return NULL;
		}
		return sDamageTypeNames[type];
	}
	const char *GameObject::getDamageTypeName(int type)
	{
		if (type < 0 || type >= MAX_DAMAGE_TYPE)
		{
			return NULL;
		}
		return sDamageTypeNames[type];
	}
	GameObject::DamageType GameObject::getDamageType(const char *typeName)
	{
		if (!typeName || typeName[0] == '\0')
		{
			return MAX_DAMAGE_TYPE;
		}
		for (int i = 0; i < MAX_DAMAGE_TYPE; i++)
		{
			if (strcmp(typeName, sDamageTypeNames[i]) == 0)
			{
				return static_cast<DamageType>(i);
			}
		}
		return MAX_DAMAGE_TYPE;
	}
	GameObject::DamageType GameObject::getDamageType(int type)
	{
		if (type < 0 || type >= MAX_DAMAGE_TYPE)
		{
			return MAX_DAMAGE_TYPE;
		}
		return static_cast<DamageType>(type);
	}
}
}
