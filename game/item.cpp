#include "item.h"

#include "inventory.h"
#include "engine.h"
#include "tile_type.h"

#include <ui/mouse_manager.h>

#include <util/utils.h>
using namespace am::util;

#include <log/logger.h>

#include <sstream>
using namespace std;

namespace am {
namespace game {

	Item::Item() :
		GameObject(),
		mGraphic(NULL),
		mGroundGraphic(NULL),
		mItemType(ItemCommon::UNKNOWN),
		/*mMinDamage(0.0f),
		mMaxDamage(0.0f),
		mArmourClass(0.0f),*/
		mInventorySizeX(1),
		mInventorySizeY(1),
		mItemLocation(GROUND)
	{
		setName("Item");
		addPassibleType(TileType::getTileType("land"));
		//setInteractive(true);
	}
	Item::~Item()
	{

	}

	void Item::setGraphic(Sprite *graphic, bool calcInvSize)
	{
		if (mGraphic)
		{
			mGraphic->removeEventListener(MOUSE_UP, this);
			removeChild(mGraphic);
		}
		mGraphic = graphic;
		graphic->setInteractive(true);

		if (calcInvSize && graphic)
		{
			graphic->addEventListener(MOUSE_UP, this);
			mInventorySizeX = static_cast<short>(ceil(graphic->getWidth() / Inventory::getSpaceSizeX()));
			mInventorySizeY = static_cast<short>(ceil(graphic->getHeight() / Inventory::getSpaceSizeY()));
		}
		updateGraphic();
	}
	Sprite *Item::getGraphic()
	{
		return mGraphic.get();
	}

	void Item::setGroundGraphic(Sprite *graphic)
	{
		if (mGroundGraphic)
		{
			mGroundGraphic->removeEventListener(MOUSE_UP, this);
			removeChild(mGroundGraphic);
		}
		mGroundGraphic = graphic;
		graphic->setInteractive(true);
		if (graphic)
		{
			mCameraOffsetX = graphic->getWidth() * 0.5f;
			mCameraOffsetY = graphic->getHeight() * 0.5f;
			graphic->addEventListener(MOUSE_UP, this);
		}
		updateGraphic();
	}
	Sprite *Item::getGroundGraphic()
	{
		return mGraphic.get();
	}

	void Item::setItemType(ItemCommon::ItemType type)
	{
		mItemType = type;
	}
	ItemCommon::ItemType Item::getItemType() const
	{
		return mItemType;
	}

	void Item::setItemTypeName(const char *name)
	{
		if (name == NULL)
		{
			return;
		}
		mItemType = ItemCommon::getItemTypeFromName(name);
	}
	const char *Item::getItemTypeName() const
	{
		return ItemCommon::ItemTypeNames[mItemType];
	}

	void Item::setItemFrom(const Item &item)
	{
		mItemType = item.mItemType;
		if (item.mGraphic.get())
		{
			setGraphic(new Sprite(item.mGraphic->getAsset()));
		}
		if (item.mGroundGraphic.get())
		{
			setGroundGraphic(new Sprite(item.mGroundGraphic->getAsset()));
		}
		mName = item.mName;
		mInventorySizeX = item.mInventorySizeX;
		mInventorySizeY = item.mInventorySizeY;
		/*mMinDamage = item.mMinDamage;
		mMaxDamage = item.mMaxDamage;
		mArmourClass = item.mArmourClass;*/
		mStatModifiers = item.mStatModifiers;
		mItemLocation = item.mItemLocation;
		mItemName = item.mItemName;
		mPrefix = item.mPrefix;
		mPostfix = item.mPostfix;
		updateFullname();
	}

	StatModifiers &Item::getStatModifiers()
	{
		return mStatModifiers;
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
	/*
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
	*/
	void Item::setItemLocation(ItemLocation location)
	{
		if (mItemLocation != location)
		{
			mItemLocation = location;
			updateGraphic();
		}
	}
	Item::ItemLocation Item::getItemLocation() const
	{
		return mItemLocation;
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
		Handle<Item> thisHandle(this);
		if (groundGraphic.get() == NULL && graphic.get() != NULL)
		{
			groundGraphic = graphic;
		}
		else if (groundGraphic.get() != NULL && graphic.get() == NULL)
		{
			graphic = groundGraphic;
		}
		if (mItemLocation == GROUND)
		{
			removeChild(graphic);
			addChild(groundGraphic);
			mCurrentGraphic = groundGraphic;
		}
		else
		{
			removeChild(groundGraphic);
			addChild(graphic);
			mCurrentGraphic = graphic;
		}
	}

	void Item::setItemFullname( const char *name, const char *prefix, const char *postfix )
	{
		mItemName = name;
		mPrefix = prefix;
		mPostfix = postfix;
		updateFullname();
	}
	void Item::setItemName( const char *name )
	{
		mItemName = name;
		updateFullname();
	}
	const char *Item::getItemName() const
	{
		return mItemName.c_str();
	}

	void Item::setPrefix( const char *prefix )
	{
		mPrefix = prefix;
		updateFullname();
	}
	const char *Item::getPrefix() const
	{
		return mPrefix.c_str();
	}

	void Item::setPostfix( const char *postfix )
	{
		mPostfix = postfix;
		updateFullname();
	}
	const char *Item::getPostfix() const
	{
		return mPostfix.c_str();
	}

	const char *Item::getFullItemName() const
	{
		return mFullname.c_str();
	}

	void Item::loadDef(JsonValue value)
	{
		if (value.getType() != JV_OBJ)
		{
			stringstream ss;
			ss << "Cannot load definition of item from type '" << value.getTypeName() << "'";
			am_log("ITEM", ss);
			return;
		}
		if (value.has("graphic", JV_STR))
		{
			setGraphic(new Sprite(value["graphic"].getCStr()), true);
		}
		if (value.has("groundGraphic", JV_STR))
		{
			setGroundGraphic(new Sprite(value["groundGraphic"].getCStr()));
		}
		if (value.has("sizeX", JV_INT))
		{
			mInventorySizeX = value["sizeX"].getInt();
			if (mInventorySizeX < 1)
			{
				mInventorySizeX = 1;
			}
		}
		if (value.has("sizeY", JV_INT))
		{
			mInventorySizeY = value["sizeY"].getInt();
			if (mInventorySizeY < 1)
			{
				mInventorySizeY = 1;
			}
		}
		/*if (value.has("minDamage", JV_INT) || value.has("minDamage", JV_FLOAT))
		{
			mMinDamage = value["minDamage"].getFloat();
		}
		if (value.has("maxDamage", JV_INT) || value.has("maxDamage", JV_FLOAT))
		{
			mMinDamage = value["maxDamage"].getFloat();
		}*/
		if (value.has("name", JV_STR))
		{
			mItemName = value["name"].getCStr();
		}
		if (value.has("prefix", JV_STR))
		{
			mPrefix = value["prefix"].getCStr();
		}
		if (value.has("postfix", JV_STR))
		{
			mPostfix = value["postfix"].getCStr();
		}
		updateFullname();

		if (value.has("itemType", JV_STR))
		{
			setItemTypeName(value["itemType"].getCStr());
		}

		if (value.has("questId", JV_INT))
		{
			setQuestItemId(value["questId"].getInt());
		}

		if (value.has("stats", JV_OBJ))
		{
			parseStats(*value["stats"].getObj(), false);
		}
		if (value.has("magicalStats", JV_OBJ))
		{
			parseStats(*value["magicalStats"].getObj(), true);
		}

		/*if (value.has("armour", JV_INT) || value.has("armour", JV_FLOAT))
		{
			setArmourClass(value["armour"].getFloat());
		}*/
	}

	void Item::parseStats(const JsonObject &stats, bool magical)
	{
		JsonObject::const_iterator iter;
		for (iter = stats.begin(); iter != stats.end(); ++iter)
		{
			Stat::StatType statType = Stat::getStatType(iter->first.c_str());
			if (statType == Stat::MAX_STAT_LENGTH)
			{
				stringstream ss;
				ss << "Unknown stat type '" << iter->first << "'";
				am_log("ITEM", ss);
				continue;
			}
			JsonType jsonType = iter->second.getType();
			float value = 0.0f;
			StatModifierType type = MOD_ADD;
			if (jsonType == JV_INT || jsonType == JV_FLOAT)
			{
				value = iter->second.getFloat();
			}
			else if (jsonType == JV_STR)
			{
				string *str = iter->second.getStr();
				int foundAdd = static_cast<int>(str->find('+'));
				int foundMulti = static_cast<int>(str->find('%'));
				if (foundAdd >= 0 && foundMulti >= 0)
				{
					type = MOD_MULTIPLY;
				}
				else if (foundMulti >= 0)
				{
					type = MOD_MULTIPLY_SET;
				}

				int i = max(0, max(foundAdd, foundMulti));

				bool parsed = Utils::fromString<float>(value, str->c_str() + i + 1);
				if (!parsed)
				{
					stringstream ss;
					ss << "Unable to parse stat '" << str->c_str() << "'";
					am_log("ITEM", ss);
				}
				else
				{
					StatModifier modifier(value, type, magical);
					mStatModifiers.addStatModifier(statType, modifier);
				}
			}
		}
	}

	void Item::updateFullname()
	{
		if (mPrefix.size() > 0)
		{
			mFullname = mPrefix + ' ' + mItemName;
		}
		else
		{
			mFullname = mItemName;
		}
		if (mPostfix.size() > 0)
		{
			mFullname += ' ' + mPostfix;
		}
	}

	float Item::getWidth()
	{
		return mCurrentGraphic->getWidth();
	}
	float Item::getHeight()
	{
		return mCurrentGraphic->getHeight();
	}

	const char *Item::getGameObjectTypeName() const
	{
		return "item";
	}

	string Item::getName() const
	{
		return string(getFullItemName());
	}

}
}
