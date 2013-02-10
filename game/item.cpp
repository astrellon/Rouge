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

	const int Item::LUA_ID = __COUNTER__;
	const char *Item::LUA_TABLENAME = "am_game_Item";

	const char *Item::sItemLocationNames[] = {
		"ground", "inventory", "hand", "MAX_LENGTH"
	};

	Item::Item() :
		GameObject(),
		mGraphic(NULL),
		mGroundGraphic(NULL),
		mItemType(ItemCommon::UNKNOWN),
		mInventorySizeX(1),
		mInventorySizeY(1),
		mItemLocation(GROUND),
		mItemValue(1),
		mPrefix(-1),
		mPostfix(-1)
	{
		setName("Item");
		addPassibleType(Engine::getEngine()->getTileType("land"));
	}
	Item::~Item()
	{

	}

	Item *Item::clone() const
	{
		Handle<Item> newItem(new Item());
		newItem->setItemFrom(*this);
		newItem->retain();
		return newItem;
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
		mItemType = ItemCommon::getItemType(name);
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
		mStatModifiers = item.mStatModifiers;
		mItemLocation = item.mItemLocation;

		mPrefix = item.mPrefix;
		mPostfix = item.mPostfix;
		mItemValue = item.mItemValue;
		//updateFullname();
		updateGraphic();
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
	
	void Item::setItemValue(unsigned int value)
	{
		mItemValue = value;
	}
	unsigned int Item::getItemValue() const
	{
		return mItemValue;
	}

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
		string nameStr(name);
		if (prefix != NULL && prefix[0] != '\0')
		{
			string prefixStr(prefix); 
			mName = prefixStr + ' ' + nameStr;
			mPrefix = static_cast<short>(prefixStr.size());
		}
		else
		{
			mName = nameStr;
			mPrefix = -1;
		}
		if (postfix != NULL && postfix[0] != '\0')
		{
			string postfixStr(postfix);
			mName += " " + postfixStr;
			mPostfix = static_cast<short>(postfixStr.size());
		}
		else
		{
			mPostfix = -1;
		}
	}
	string Item::getFullItemName() const
	{
		return mName;
	}
	void Item::setItemName( const char *name )
	{
		short prefix = mPrefix;
		short postfix = mPostfix;
		getPrePostfix(prefix, postfix);
		mName.replace(prefix, postfix, name);
		if (mPostfix >= 0)
		{
			postfix = strlen(name) + prefix + 1;
		}
	}
	string Item::getItemName() const
	{
		short prefix = mPrefix;
		short postfix = mPostfix;
		getPrePostfix(prefix, postfix);
		return mName.substr(prefix, postfix - prefix);
	}

	void Item::setPrefix( const char *prefix )
	{
		if (prefix == NULL)
		{
			prefix = "";
		}
		string prefixStr(prefix);
		if (prefixStr.size() == 0)
		{
			if (mPrefix < 0)
			{
				return;
			}
			mName.erase(0, mPrefix + 1);
			mPrefix = -1;
			return;
		}
		
		if (mPrefix < 0) 
		{
			mName = prefixStr + ' ' + mName;
		}
		else
		{
			mName.replace(0, mPrefix, prefixStr);
		}
		mPrefix = static_cast<short>(prefixStr.size());
	}
	string Item::getPrefix() const
	{
		if (mPrefix < 0)
		{
			return string();
		}
		return mName.substr(0, mPrefix);
	}

	void Item::setPostfix( const char *postfix )
	{
		if (postfix == NULL)
		{
			postfix = "";
		}
		string postfixStr(postfix);
		if (postfixStr.size() == 0)
		{
			if (mPostfix < 0)
			{
				return;
			}
			mName.erase(mName.size() - mPostfix - 1);
			mPostfix = -1;
			return;
		}
		if (mPostfix < 0)
		{
			mName = mName + ' ' + postfixStr;
		}
		else
		{
			mName.replace(mName.size() - mPostfix, mName.size(), postfix);
		}
		mPostfix = static_cast<short>(postfixStr.size());
	}
	string Item::getPostfix() const
	{
		if (mPostfix < 0)
		{
			return string();
		}
		return mName.substr(mName.size() - mPostfix);
	}

	void Item::loadDef(LuaState &lua)
	{
		if (!lua_istable(lua, -1))
		{
			stringstream ss;
			ss << "Cannot load definition of item from type '" << lua_typename(lua, -1) << "'";
			am_log("ITEM", ss);
			return;
		}
		if (lua.isTableString("graphic"))
		{
			setGraphic(new Sprite(lua_tostring(lua, -1)), true);
			lua.pop(1);
		}
		if (lua.isTableString("groundGraphic"))
		{
			setGroundGraphic(new Sprite(lua_tostring(lua, -1)));
			lua.pop(1);
		}
		if (lua.isTableNumber("sizeX"))
		{
			mInventorySizeX = lua.toInteger();
			if (mInventorySizeX < 1)
			{
				mInventorySizeX = 1;
			}
		}
		if (lua.isTableNumber("sizeY"))
		{
			mInventorySizeY = lua.toInteger();
			if (mInventorySizeY < 1)
			{
				mInventorySizeY = 1;
			}
		}
		string name;
		string prefix;
		string postfix;
		if (lua.isTableString("name"))
		{
			//mItemName = lua_tostring(lua, -1);
			name = lua_tostring(lua, -1);
			lua.pop(1);
		}
		if (lua.isTableString("prefix"))
		{
			//mPrefix = lua_tostring(lua, -1);
			prefix = lua_tostring(lua, -1);
			lua.pop(1);
		}
		if (lua.isTableString("postfix"))
		{
			//mPostfix = lua_tostring(lua, -1);
			postfix = lua_tostring(lua, -1);
			lua.pop(1);
		}
		//updateFullname();
		setItemFullname(name.c_str(), prefix.c_str(), postfix.c_str());

		if (lua.isTableString("itemType"))
		{
			setItemTypeName(lua_tostring(lua, -1));
			lua.pop(1);
		}
		if (lua.isTableNumber("questId"))
		{
			setQuestItemId(lua.toInteger());
		}
		if (lua.isTableTable("stats"))
		{
			parseStats(lua, false);
			lua.pop(1);
		}
		if (lua.isTableTable("magicalStats"))
		{
			parseStats(lua, true);
			lua.pop(1);
		}
		if (lua.isTableNumber("value"))
		{
			setItemValue(lua.toInteger());
		}
	}

	/*void Item::parseStats(const JsonObject &stats, bool magical)
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
					type = MOD_SET;
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
	}*/
	void Item::parseStats(LuaState &lua, bool magical)
	{
		if (!lua_istable(lua, -1))
		{
			return;
		}
		lua_pushnil(lua);
		while (lua_next(lua, -2) != 0)
		{
			if (!lua_isstring(lua, -2))
			{
				lua.pop(1);
				continue;
			}
			Stat::StatType statType = Stat::getStatType(lua_tostring(lua, -2));
			if (statType == Stat::MAX_STAT_LENGTH)
			{
				stringstream ss;
				ss << "Unknown stat type '" << lua_tostring(lua, -2) << "'";
				am_log("ITEM", ss);
				lua.pop(1);
				continue;
			}

			float value = 0.0f;
			StatModifierType type = MOD_ADD;
			if (lua_isnumber(lua, -1))
			{
				value = lua_tofloat(lua, -1);
			}
			else if (lua_isstring(lua, -1))
			{
				string str = lua_tostring(lua, -1);
				int foundAdd = static_cast<int>(str.find('+'));
				int foundMulti = static_cast<int>(str.find('%'));
				if (foundAdd >= 0 && foundMulti >= 0)
				{
					type = MOD_MULTIPLY;
				}
				else if (foundMulti >= 0)
				{
					type = MOD_SET;
				}

				int i = max(0, max(foundAdd, foundMulti));

				bool parsed = Utils::fromString<float>(value, str.c_str() + i + 1);
				if (!parsed)
				{
					stringstream ss;
					ss << "Unable to parse stat '" << str.c_str() << "'";
					am_log("ITEM", ss);
				}
				else
				{
					StatModifier modifier(value, type, magical);
					mStatModifiers.addStatModifier(statType, modifier);
				}
			}
			lua.pop(1);
		}
	}

	void Item::loadFromLua(const char *filename)
	{
		LuaState lua(false);
		stringstream ss;
		ss << "data/items/" << filename << ".lua";
		if (!lua.loadFile(ss.str().c_str()))
		{
			stringstream errss;
			errss << "Error loading item lua file '" << filename << "'";
			am_log("ITEM", errss);
			lua.logStack("ITEMLUA");
			lua.close();
			return;
		}
		lua_getglobal(lua, "item");
		if (!lua_istable(lua, -1))
		{
			stringstream errss;
			errss << "Did not find global item table: " << lua_typename(lua, -1);
			am_log("ITEMLUA", errss);
			lua.close();
		}
		loadDef(lua);
		lua.close();
	}

	/*void Item::updateFullname()
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
	}*/

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

	Item::ItemLocation Item::getItemLocationType(const char *typeName)
	{
		for (int i = 0; i < MAX_LENGTH; i++)
		{
			if (strcmp(sItemLocationNames[i], typeName) == 0)
			{
				return static_cast<ItemLocation>(i);
			}
		}
		return MAX_LENGTH;
	}
	Item::ItemLocation Item::getItemLocationType(int typeValue)
	{
		if (typeValue < 0 || typeValue >= MAX_LENGTH)
		{
			return MAX_LENGTH;
		}
		return static_cast<ItemLocation>(typeValue);
	}

	const char *Item::getItemLocationTypeName(Item::ItemLocation location)
	{
		if (location < 0 || location >= Item::MAX_LENGTH)
		{
			return NULL;
		}
		return sItemLocationNames[location];
	}

	void Item::getPrePostfix(short &prefix, short &postfix) const
	{
		if (prefix < 0) {
			prefix = 0;
		}
		else 
		{
			prefix++;
		}
		if (postfix < 0) {
			postfix = mName.size();
		}
		else
		{
			postfix = mName.size() - postfix - 1;
		}
	}

}
}
