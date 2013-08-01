#include "item.h"

#include "inventory.h"
#include "engine.h"
#include "tile_type.h"

#include <ui/mouse_manager.h>

#include <util/utils.h>
#include <util/data_table.h>
#include <util/data_string.h>
#include <util/data_number.h>
using namespace am::util;

#include <log/logger.h>

#include <sstream>
using namespace std;

namespace am {
namespace game {

	const int Item::LUA_ID = 0x0B;
	const char *Item::LUA_TABLENAME = "am_game_Item";

	const char *Item::sItemLocationNames[] = {
		"ground", "inventory", "hand", "MAX_LENGTH"
	};

	Item::Item() :
		GameObject(),
		mItemType(ItemCommon::UNKNOWN),
		mInventorySizeX(1),
		mInventorySizeY(1),
		mItemLocation(GROUND),
		mItemValue(1),
		mPrefix(-1),
		mPostfix(-1),
		mQuestItemId(-1),
		mBodyPartsRequired(1),
		mStatModifiers(new StatModifiers()),
		mStatModifiersSelf(new StatModifiers()),
		mBaseDamageType(BLUNT)
	{
		setName("Item");
		addPassibleType(Engine::getEngine()->getTileType("land"));
	}
	Item::Item(const Item &copy) :
		GameObject(copy),
		mItemType(copy.mItemType),
		mInventorySizeX(copy.mInventorySizeX),
		mInventorySizeY(copy.mInventorySizeY),
		mItemLocation(copy.mItemLocation),
		mItemValue(copy.mItemValue),
		mPrefix(copy.mPrefix),
		mPostfix(copy.mPostfix),
		mQuestItemId(copy.mQuestItemId),
		mBodyPartsRequired(copy.mBodyPartsRequired),
		mBaseDamageType(copy.mBaseDamageType)
	{
		if (copy.mGraphic)
		{
			mGraphic = new Sprite(*copy.mGraphic);
		}
		if (copy.mGroundGraphic)
		{
			mGroundGraphic = new Sprite(*copy.mGroundGraphic);
		}
		if (copy.mStatModifiers)
		{
			mStatModifiers = new StatModifiers(*copy.mStatModifiers);
		}
		else
		{
			mStatModifiers = new StatModifiers();
		}
		if (copy.mStatModifiersSelf)
		{
			mStatModifiersSelf = new StatModifiers(*copy.mStatModifiersSelf);
		}
		else
		{
			mStatModifiersSelf = new StatModifiers();
		}
		retain();
		updateGraphic();
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
		if (name == nullptr)
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

		mStatModifiers = new StatModifiers(*item.mStatModifiers);
		mStatModifiersSelf = new StatModifiers(*item.mStatModifiersSelf);

		mPrefix = item.mPrefix;
		mPostfix = item.mPostfix;
		mItemValue = item.mItemValue;
		//updateFullname();
		updateGraphic();
	}

	StatModifiers *Item::getStatModifiers()
	{
		return mStatModifiers;
	}
	StatModifiers *Item::getStatModifiersSelf()
	{
		return mStatModifiersSelf;
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
		if (groundGraphic.get() == nullptr && graphic.get() != nullptr)
		{
			groundGraphic = graphic;
		}
		else if (groundGraphic.get() != nullptr && graphic.get() == nullptr)
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
		if (prefix != nullptr && prefix[0] != '\0')
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
		if (postfix != nullptr && postfix[0] != '\0')
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
		if (prefix == nullptr)
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
		if (postfix == nullptr)
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
			return nullptr;
		}
		return sItemLocationNames[location];
	}

	bool Item::addBodyPartType(BodyPartType::PartType type)
	{
		if (!hasBodyPartType(type))
		{
			mEquipableTo.push_back(type);
			return true;
		}
		return false;
	}
	bool Item::removeBodyPartType(BodyPartType::PartType type)
	{
		int index = Utils::find(mEquipableTo, type);
		if (index >= 0)
		{
			mEquipableTo.erase(mEquipableTo.begin() + index);
			return true;
		}
		return false;
	}
	bool Item::hasBodyPartType(BodyPartType::PartType type)
	{
		return Utils::listContains(mEquipableTo, type);
	}
	const BodyPartType::TypeList &Item::getBodyPartTypeList() const
	{
		return mEquipableTo;
	}

	void Item::setBodyPartsRequired(unsigned int num)
	{
		mBodyPartsRequired = num;
	}
	unsigned int Item::getBodyPartsRequired() const
	{
		return mBodyPartsRequired;
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

	data::IData *Item::serialise()
	{
		data::IData *obj_output = GameObject::serialise();
		data::Table *output = dynamic_cast<data::Table *>(obj_output);
		if (!output)
		{
			am_log("ERROR", "Save game object from GameObject not a data::Map!");
			return nullptr;
		}

		output->at("itemType", ItemCommon::getItemTypeName(mItemType));
		output->at("itemLocation", getItemLocationTypeName(mItemLocation));
		output->at("inventorySizeX", mInventorySizeX);
		output->at("inventorySizeY", mInventorySizeY);
		output->at("questItemId", mQuestItemId);

		output->at("itemValue", static_cast<int>(mItemValue));

		if (mGraphic)
		{
			output->at("graphic", mGraphic->serialise());
		}
		if (mGroundGraphic)
		{
			output->at("groundGraphic", mGroundGraphic->serialise());
		}
		output->at("statModifiers", mStatModifiers->serialise());
		output->at("statModifiersSelf", mStatModifiersSelf->serialise());
		
		output->at("equipableTo", BodyPartType::serialiseTypeList(mEquipableTo));

		output->at("bodyPartsRequired", mBodyPartsRequired);

		return output;
	}
	int Item::deserialise(LoadingState *state, data::IData *data)
	{
		int gameObjDeserialise = GameObject::deserialise(state, data);
		if (gameObjDeserialise < 1)
		{
			return gameObjDeserialise;
		}
		Handle<data::Table> dataMap(data::Table::checkDataType(data, "item"));
		if (!dataMap)
		{
			return -1;
		}

		Handle<data::IData> tempData(dataMap->at("graphic"));
		if (tempData)
		{
			mGraphic = new Sprite();
			mGraphic->deserialise(state, tempData);
		}
		tempData = dataMap->at("groundGraphic");
		if (tempData)
		{
			mGroundGraphic = new Sprite();
			mGroundGraphic->deserialise(state, tempData);
		}

		Handle<data::String> str(dataMap->at<data::String>("itemType"));
		if (str)
		{
			setItemType(ItemCommon::getItemType(str->string()));
		}
		str = dataMap->at<data::String>("itemLocation");
		if (str)
		{
			setItemLocation(getItemLocationType(str->string()));
		}
		updateGraphic();

		Handle<data::Number> num(dataMap->at<data::Number>("inventorySizeX"));
		if (num)
		{
			mInventorySizeX = num->value<short>();
		}
		num = dataMap->at<data::Number>("inventorySizeY");
		if (num)
		{
			mInventorySizeY = num->value<short>();
		}

		num = dataMap->at<data::Number>("questItemId");
		if (num)
		{
			setQuestItemId(num->valuei());
		}

		num = dataMap->at<data::Number>("itemValue");
		if (num)
		{
			setItemValue(num->value<unsigned int>());
		}

		tempData = dataMap->at<data::IData>("statModifiers");
		if (tempData)
		{
			mStatModifiers->deserialise(state, tempData);
		}
		tempData = dataMap->at<data::IData>("statModifiersSelf");
		if (tempData)
		{
			mStatModifiersSelf->deserialise(state, tempData);
		}

		tempData = dataMap->at<data::IData>("equipableTo");
		if (tempData)
		{
			BodyPartType::deserialiseTypeList(tempData, mEquipableTo);
		}

		num = dataMap->at<data::Number>("bodyPartsRequired");
		if (num)
		{
			setBodyPartsRequired(num->value<unsigned int>());
		}
		return 1;
	}

	void Item::onLevelUp()
	{
		Handle<Event> e(new Event("level_change"));
		fireEvent<Event>(e);
	}
	void Item::onExperienceChange()
	{
		Handle<Event> e(new Event("experience_change"));
		fireEvent<Event>(e);
	}

}
}
