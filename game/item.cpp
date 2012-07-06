#include "item.h"

#include "engine.h"

#include <sstream>
using namespace std;

namespace am {
namespace game {

	Item::Item() :
		GameObject(),
		mGraphic(NULL),
		mGroundGraphic(NULL),
		mItemType(UNKNOWN),
		mMinDamage(0.0f),
		mMaxDamage(0.0f),
		mArmourClass(0.0f),
		mInventorySizeX(1),
		mInventorySizeY(1),
		mOnGround(false)
	{
		setName("Item");
	}
	Item::~Item()
	{

	}

	void Item::setGraphic(Sprite *graphic)
	{
		if (mGraphic.get())
		{
			removeChild(mGraphic.get());
		}
		mGraphic = graphic;
		updateGraphic();
	}
	Sprite *Item::getGraphic()
	{
		return mGraphic.get();
	}

	void Item::setGroundGraphic(Sprite *graphic)
	{
		if (mGroundGraphic.get())
		{
			removeChild(mGroundGraphic.get());
		}
		mGroundGraphic = graphic;
		if (graphic)
		{
			mCameraOffsetX = graphic->getWidth() * 0.5f;
			mCameraOffsetY = graphic->getHeight() * 0.5f;
		}
		updateGraphic();
	}
	Sprite *Item::getGroundGraphic()
	{
		return mGraphic.get();
	}

	void Item::update(float dt)
	{
		
	}

	void Item::setItemType(ItemType type)
	{
		mItemType = type;
	}
	ItemType Item::getItemType() const
	{
		return mItemType;
	}

	void Item::setItemFrom(const Item &item)
	{
		mItemType = item.mItemType;
		setGraphic(item.mGraphic.get());
		setGroundGraphic(item.mGroundGraphic.get());
		mName = item.mName;
		mInventorySizeX = item.mInventorySizeX;
		mInventorySizeY = item.mInventorySizeY;
		mMinDamage = item.mMinDamage;
		mMaxDamage = item.mMaxDamage;
		mArmourClass = item.mArmourClass;
	}

	void Item::setInventorySize(short sizeX, short sizeY)
	{
		mInventorySizeX = sizeX;
		mInventorySizeY = sizeY;
	}
	short Item::getInventorySizeX() const
	{
		return mInventorySizeX;
	}
	short Item::getInventorySizeY() const
	{
		return mInventorySizeY;
	}

	void Item::setMinDamage(float dmg)
	{
		mMinDamage = dmg;
	}
	float Item::getMinDamage() const
	{
		return mMinDamage;
	}

	void Item::setMaxDamage(float dmg)
	{
		mMaxDamage = dmg;
	}
	float Item::getMaxDamage() const
	{
		return mMinDamage;
	}

	void Item::setArmourClass(float ac)
	{
		mArmourClass = ac;
	}
	float Item::getArmourClass() const
	{
		return mArmourClass;
	}

	void Item::setOnGround(bool ground)
	{
		if (ground != mOnGround)
		{
			mOnGround = ground;
			updateGraphic();
		}
	}
	bool Item::isOnGround() const
	{
		return mOnGround;
	}

	void Item::setQuestItemId(int questItemId)
	{
		mQuestItemId = questItemId;
	}
	int Item::getQuestItemId() const
	{
		return mQuestItemId;
	}
	bool Item::isQuestItem() const
	{
		return mQuestItemId > 0;
	}

	void Item::updateGraphic()
	{
		Handle<Sprite> graphic = mGraphic;
		Handle<Sprite> groundGraphic = mGroundGraphic;
		if (groundGraphic.get() == NULL && graphic.get() != NULL)
		{
			groundGraphic = graphic;
		}
		else if (groundGraphic.get() != NULL && graphic.get() == NULL)
		{
			graphic = groundGraphic;
		}
		if (mOnGround)
		{
			addChild(groundGraphic.get());
			removeChild(graphic.get());
		}
		else
		{
			addChild(graphic.get());
			removeChild(groundGraphic.get());
		}
	}

}
}
