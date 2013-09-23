#include "door.h"

#include "map.h"

#include <util/data_table.h>
#include <util/data_number.h>
#include <util/data_boolean.h>
#include <util/data_string.h>
#include <util/utils.h>
using namespace am::util;

#include <math/math.h>

#include <sstream>
using namespace std;

#include <log/logger.h>

#include <gl.h>
#include <gfx/gfx_texture.h>

namespace am {
namespace game {

	const int Door::LUA_ID = 0x26;
	const char *Door::LUA_TABLENAME = "am_game_Door";

	Door::Door() :
		GameObject(),
		mGraphic(nullptr),
		mOpened(false)
	{
		mFixedToGrid = true;
		setName("Door");
		addEventListener(MOUSE_UP, this);
	}
	Door::Door(const Door &copy) :
		GameObject(copy),
		mOpened(copy.mOpened)
	{
		if (copy.mGraphic)
		{
			// Find our new graphic if there was one.
			for (int i = 0; i < getNumChildren(); i++)
			{
				Sprite *temp = dynamic_cast<Sprite *>(getChildAt(i));
				if (temp && temp->getAsset() == copy.mGraphic->getAsset())
				{
					mGraphic = temp;
				}
			}
		}
	}
	Door::~Door()
	{
		removeEventListener(MOUSE_UP, this);
	}

	void Door::setGraphic(Sprite *graphic, bool calcCameraOffset)
	{
		Handle<Sprite> currentGraphic(mGraphic);
		if (mGraphic)
		{
			removeChild(mGraphic);
		}
		mGraphic = graphic;
		if (graphic)
		{
			graphic->setInteractive(true);
			addChild(mGraphic);
		}
		if (graphic && calcCameraOffset)
		{
			mCameraOffsetX = graphic->getWidth() * 0.5f;
			mCameraOffsetY = graphic->getHeight() * 0.5f;
		}
		updateGraphic();
	}
	Sprite *Door::getGraphic() const
	{
		return mGraphic;
	}
	   
	void Door::update(float dt)
	{
		
	}
	bool Door::onGameTick(float dt)
	{
		return false;
	}
	
	void Door::receiveDamage(float damage)
	{
		
	}

	void Door::setOpened(bool opened)
	{
		if (mOpened != opened)
		{
			mOpened = opened;
			updateGraphic();
		}
	}
	bool Door::isOpened() const
	{
		return mOpened;
	}
	
	float Door::getWidth()
	{
		return mGraphic->getWidth();
	}
	float Door::getHeight()
	{
		return mGraphic->getHeight();
	}

	const char *Door::getGameObjectTypeName() const
	{
		return "door";
	}
	GameObject::GameObjectType Door::getGameObjectType() const
	{
		return DOOR;
	}

	data::IData *Door::serialise()
	{
		data::IData *obj_output = Door::serialise();
		data::Table *output = dynamic_cast<data::Table *>(obj_output);
		if (!output)
		{
			am_log("ERROR", "Save door from GameObject not a data::Map!");
			return nullptr;
		}

		output->at("opened", mOpened);

		if (mGraphic)
		{
			output->at("graphic", mGraphic->serialise());
		}
		
		return output;
	}
	int Door::deserialise(LoadingState *state, data::IData *data)
	{
		int loadResult = GameObject::deserialise(state, data);
		if (loadResult < 1)
		{
			return loadResult;
		}

		Handle<data::Table> dataMap(dynamic_cast<data::Table *>(data));
		if (!dataMap)
		{	// Shouldn't happen due to GameObject::deserialise
			return -1;
		}

		Handle<data::IData> tempData(dataMap->at("graphic"));
		if (tempData)
		{
			Sprite *graphic = new Sprite();
			graphic->deserialise(state, tempData);
			setGraphic(graphic, false);
		}

		return 1;
	}

	void Door::updateGraphic()
	{
		if (!mGraphic)
		{
			return;
		}

		if (mOpened)
		{
			mGraphic->setSubWindowFrame(1);
		}
		else
		{
			mGraphic->setSubWindowFrame(0);
		}
	}
}
}
