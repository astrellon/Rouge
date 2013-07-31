#include "game_object.h"

#include <gl.h>

#include "map.h"

#include "engine.h"
#include "tile_type.h"
#include "game.h"
#include "dialogue_component.h"
#include "loading_state.h"

#include <math/math.h>

#include <util/data_table.h>
#include <util/data_boolean.h>
#include <util/data_string.h>
#include <util/data_number.h>

#include <sfx/sfx_source_point.h>
#include <sfx/sfx_source_area.h>

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
		mMap(nullptr),
		mOriginalMap(nullptr)
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
		mMap(nullptr),
		mOriginalMap(nullptr)
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
			mMap = nullptr;
			map->release();
		}
		if (mOriginalMap)
		{
			Map *map = mOriginalMap;
			mOriginalMap = nullptr;
			map->release();
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
		if (mSoundSource)
		{
			mSoundSource->setPosition(x, y);
		}
		if (setDraw)
		{
			if (mFixedToGrid)
			{
				Engine *engine = Engine::getEngine();
				x = am::math::round(x * engine->getGridSizeResp()) * engine->getGridSize();
				y = am::math::round(y * engine->getGridSizeResp()) * engine->getGridSize();
			}
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
		float flocX = Engine::getEngine()->getGridSize() * static_cast<float>(x);
		float flocY = Engine::getEngine()->getGridSize() * static_cast<float>(y);
		int locX = static_cast<int>(flocX);
		int locY = static_cast<int>(flocY);
		if (!mMap || (mMap && mMap->isValidGridLocation(x, y, this)))
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
	int GameObject::getGridLocationX() const
	{
		return am::math::round(mLocationX * Engine::getEngine()->getGridSizeResp());
	}
	int GameObject::getGridLocationY() const
	{
		return am::math::round(mLocationY * Engine::getEngine()->getGridSizeResp());
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

	bool GameObject::move(float x, float y)
	{
		const Engine *engine = Engine::getEngine();
		int gridX = static_cast<int>(mLocationX * engine->getGridSizeResp());
		int gridY = static_cast<int>(mLocationY * engine->getGridSizeResp());
		int newGridX = static_cast<int>((mLocationX + x) * engine->getGridSizeResp());
		int newGridY = static_cast<int>((mLocationY + y) * engine->getGridSizeResp());
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
			//if (!mMap->isValidGridLocation(newGridX, newGridY, this))

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

	/*void GameObject::talkTo(GameObject *other)
	{
		if (mDialogueComp.get() != nullptr)
		{
			mDialogueComp->talkTo(other);
		}
		//mTalkingTo = other;
		//Handle<DialogueEvent> e(new DialogueEvent(other->getStartDialogue()));
		//fireEvent<DialogueEvent>(e);
	}
	void GameObject::talkTo(GameObject *other, Dialogue *diag)
	{
		if (mDialogueComp.get() != nullptr)
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

	ISource *GameObject::getSource(bool create)
	{
		if (create && !mSoundSource)
		{
			setSource(new SourcePoint());
		}
		return mSoundSource;
	}
	void GameObject::setSource(ISource *source)
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
			am_log("LOADERR", "Loading game object as loading state or data was nullptr");
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
			mGameId = str->string();
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
				Handle<TileType> tileType(engine->getTileType(tileTypeName));
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
		stringstream ss;
		ss << "Setting sound source position: " << mLocationX << ", " << mLocationY;
		am_log("SFX", ss);
		mSoundSource->setPosition(mLocationX, mLocationY);
	}

	const char *GameObject::getGameObjectTypeName() const
	{
		return "gameobject";
	}

	void GameObject::postRender(float dt)
	{
		if (mSoundSource)
		{
			SourceArea *area = dynamic_cast<SourceArea *>(mSoundSource.get());
			if (area)
			{
				Vector2f rel = area->getPosition().sub(mLocationX, mLocationY);
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

				Vector2f relPos = area->getClosestPosition().sub(mLocationX, mLocationY);
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
}
}
