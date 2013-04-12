#include "character.h"

#include "engine.h"
#include "map.h"
#include "race.h"

#include <ui/equip_event.h>

#include <util/data_map.h>
using namespace am::util;

#include <sstream>
using namespace std;

#include <log/logger.h>

namespace am {
namespace game {

	const int Character::LUA_ID = 0x04;
	const char *Character::LUA_TABLENAME = "am_game_Character";

	Character::Character() :
		GameObject(),
		Levelable(),
		mGraphic(NULL),
		mController(NULL),
		mMoveX(0),
		mMoveY(0),
		mAge(1.0f),
		mGender(Gender::MALE),
		mRace(NULL),
		mCoinPurse(new CoinPurse())
	{
		setName("Character");
		mPickupReach = Engine::getEngine()->getGridXSize() * 1.5f;
		mMaxLevel = 50;
		addEventListener(MOUSE_UP, this);

		mInventory = new Inventory(10, 6);
		mStats.setAttachedTo(this);
	}
	Character::Character(const Character &copy) :
		GameObject(copy),
		Levelable(copy),
		mController(copy.mController),
		mAge(copy.mAge),
		mGender(copy.mGender),
		mRace(copy.mRace),
		mPickupReach(copy.mPickupReach),
		mCoinPurse(new CoinPurse(*copy.mCoinPurse)),
		mMoveX(0),
		mMoveY(0)
	{
		// Copy BodyParts, Inventory, Stats
		if (copy.mGraphic)
		{
			setGraphic(new Sprite(*copy.mGraphic), false);
		}
		mStats = Stats(copy.mStats);
		mStats.setAttachedTo(this);

		mInventory = new Inventory(*copy.mInventory);
		for (auto iter = copy.mBodyParts.begin(); iter != copy.mBodyParts.end(); ++iter)
		{
			addBodyPart(new BodyPart(*iter->second));
		}
	}
	Character::~Character()
	{
		if (mController)
		{
			mController->detach();
		}
	}

	void Character::setGraphic(Sprite *graphic, bool calcCameraOffset)
	{
		if (mGraphic.get())
		{
			removeChild(mGraphic.get());
		}
		mGraphic = graphic;
		if (graphic)
		{
			addChild(graphic);
			// Aim for head-ish area.
			if (calcCameraOffset)
			{
				mCameraOffsetX = graphic->getWidth() * 0.5f;
				mCameraOffsetY = graphic->getHeight() * 0.3f;
			}
		}
	}
	Sprite *Character::getGraphic()
	{
		return mGraphic.get();
	}

	void Character::update(float dt)
	{
		if (mController.get())
		{
			mController->update(this, dt);
		}
		if (mMoveX != 0 || mMoveY != 0)
		{
			Engine *engine = Engine::getEngine();
			moveGrid(mMoveX, mMoveY);
			//float dx = engine->getGridXSize() * static_cast<float>(mMoveX);
			//float dy = engine->getGridYSize() * static_cast<float>(mMoveY);

			//move(dx, dy);
		}
	}

	void Character::setController(IController *controller)
	{
		mController = controller;
	}
	IController *Character::getController()
	{
		return mController;
	}
	/*
	void Character::move(float x, float y)
	{
		//int gridX = static_cast<int>((mLocationX + x) * Engine::getEngine()->getGridXSizeResp());
		//int gridY = static_cast<int>((mLocationY + y) * Engine::getEngine()->getGridXSizeResp());
		setLocation(mLocationX + x, mLocationY + y);
	}
	*/
	void Character::setMoveVector(int x, int y)
	{
		mMoveX = x;
		mMoveY = y;
	}
	int Character::getMoveVectorX() const
	{
		return mMoveX;
	}
	int Character::getMoveVectorY() const
	{
		return mMoveY;
	}

	void Character::setPickupReach(float reach)
	{
		if (reach < 0.0f) {
			reach = -reach;
		}
		mPickupReach = reach;
	}
	float Character::getPickupReach() const
	{
		return mPickupReach;
	}

	float Character::getWidth()
	{
		return mGraphic->getWidth();
	}
	float Character::getHeight()
	{
		return mGraphic->getHeight();
	}

	Stats &Character::getStats()
	{
		return mStats;
	}

	bool Character::addBodyPart(BodyPart *part)
	{
		if (part == NULL)
		{
			return false;
		}
		string name = part->getName();
		BodyPart::BodyPartMap::const_iterator iter = mBodyParts.find(name);
		if (iter == mBodyParts.end())
		{
			Item *equipped = part->getEqippedItem();
			if (equipped != NULL)
			{
				_equipItem(equipped, name.c_str());
			}
			mBodyParts[name] = part;
			return true;
		}
		stringstream ss;
		ss << "Body part '" << name << "' already on character '" << getName() << "'";
		am_log("CHAR", ss);
		return false;
	}
	bool Character::removeBodyPart(BodyPart *part)
	{
		if (part == NULL)
		{
			return false;
		}
		return Character::removeBodyPart(part->getName());
	}
	bool Character::removeBodyPart(const char *partName)
	{
		if (partName == NULL)
		{
			return false;
		}
		BodyPart::BodyPartMap::const_iterator iter = mBodyParts.find(partName);
		if (iter != mBodyParts.end())
		{
			Item *equipped = iter->second->getEqippedItem();
			if (equipped != NULL)
			{
				_unequipItem(equipped, partName);
			}
			mBodyParts.erase(iter);
			return true;
		}
		return false;
	}
	bool Character::hasBodyPart(BodyPart *part) const
	{
		if (part == NULL)
		{
			return false;
		}
		return hasBodyPart(part->getName());
	}
	bool Character::hasBodyPart(const char *partName) const
	{
		if (partName == NULL)
		{
			return false;
		}
		BodyPart::BodyPartMap::const_iterator iter = mBodyParts.find(partName);
		if (iter == mBodyParts.end())
		{
			return false;
		}
		return true;
	}
	const BodyPart::BodyPartMap &Character::getBodyParts() const
	{
		return mBodyParts;
	}
	
	bool Character::equipItem(Item *item, const char *bodyPart)
	{
		if (item == NULL || bodyPart == NULL || bodyPart[0] == '\0')
		{
			return false;
		}
		string name = bodyPart;
		BodyPart::BodyPartMap::iterator iter = mBodyParts.find(name);
		if (iter == mBodyParts.end())
		{
			stringstream ss;
			ss << "Cannot equip item '" << item->getFullItemName() << "' on to '";
			ss << getName() << "' because they do not have a '" << name << "'";
			am_log("CHAR", ss);
			return false;
		}
		Item *alreadyEquipped = iter->second->getEqippedItem();
		if (alreadyEquipped == NULL)
		{
			iter->second->setEquippedItem(item);
			_equipItem(item, bodyPart);
			return true;
		}
		stringstream ss;
		ss << "'" << getName() << "' already has '" << alreadyEquipped->getFullItemName();
		ss << "' equipped on " << name;
		am_log("CHAR", ss);
		return false;
	}
	bool Character::unequipItem(const char *bodyPart)
	{
		if (bodyPart == NULL || bodyPart[0] == '\0')
		{
			return false;
		}
		string name = bodyPart;
		BodyPart::BodyPartMap::iterator iter = mBodyParts.find(name);
		if (iter == mBodyParts.end())
		{
			return false;
		}
		Item *equipped = iter->second->getEqippedItem();
		if (equipped != NULL)
		{
			_unequipItem(equipped, bodyPart);
		}
		iter->second->setEquippedItem(NULL);
		return true;
	}
	Item *Character::getEquipped(const char *bodyPart) const
	{
		if (bodyPart == NULL || bodyPart[0] == '\0')
		{
			return NULL;
		}
		string name = bodyPart;
		BodyPart::BodyPartMap::const_iterator iter = mBodyParts.find(name);
		if (iter == mBodyParts.end())
		{
			return NULL;
		}
		return iter->second->getEqippedItem();
	}

	Inventory *Character::getInventory()
	{
		return mInventory;
	}

	int Character::pickupItem(Item *item)
	{
		if (item == NULL)
		{
			return 0;
		}
		if (item->getItemLocation() == Item::GROUND)
		{
			float dx = item->getLocationX() - getLocationX();
			float dy = item->getLocationY() - getLocationY();
			if (dx > mPickupReach || dx < -mPickupReach || dy > mPickupReach || dy < -mPickupReach)
			{
				// Too far away.
				return -1;
			}
		}
		// There may not be space so this can still return false.
		return addItem(item) ? 1 : -2;
	}
	bool Character::addItem(Item *item)
	{
		if (item == NULL)
		{
			return false;
		}
		if (item->getItemType() == ItemCommon::GOLD)
		{
			unsigned int value = item->getItemValue();
			if (mCoinPurse->canAddCoin(value))
			{
				mCoinPurse->addCoin(value);
				return true;
			}
		}
		return mInventory->addItem(item);
	}
	bool Character::removeItem(Item *item)
	{
		if (item == NULL)
		{
			return false;
		}

		return mInventory->removeItem(item);
	}
	bool Character::hasItem(const Item *item) const
	{
		if (item == NULL)
		{
			return false;
		}

		return mInventory->hasItem(item);
	}
	int Character::dropItem(Item *item)
	{
		return dropItem(item, getLocationX(), getLocationY());
	}
	int Character::dropItem(Item *item, float x, float y)
	{
		if (item == NULL || mMap == NULL ||
			x < 0 || y < 0 ||
			x >= mMap->getWidth() ||
			y >= mMap->getHeight())
		{
			am_log("CHAR", "Off map");
			return 0;
		}

		float dx = getLocationX() - x;
		float dy = getLocationY() - y;
		if (mPickupReach > 0.0f && (dx < -mPickupReach || dx > mPickupReach || dy < -mPickupReach || dy > mPickupReach))
		{
			am_log("CHAR", "Too far away");
			return -1;
		}

		if (!mMap->isValidLocation(x, y, item))
		{
			am_log("CHAR", "Invalid location for item");
			return -2;
		}
		//item->setGridLocation(gridX, gridY);
		item->setLocation(x, y);
		mMap->addGameObject(item);
		item->setItemLocation(Item::GROUND);

		mInventory->removeItem(item);
		return 1;
	}

	const char *Character::getGameObjectTypeName() const
	{
		return "character";
	}

	void Character::setAge(float age)
	{
		mAge = age;
	}
	float Character::getAge() const
	{
		return mAge;
	}

	void Character::setRace(Race *race)
	{
		if (!race)
		{
			mRace = Engine::getEngine()->getUnknownRace();
		}
		else
		{
			mRace = race;
		}
	}
	Race *Character::getRace() const
	{
		return mRace;
	}

	void Character::setGender(Gender::GenderType gender)
	{
		mGender = gender;
	}
	Gender::GenderType Character::getGender() const
	{
		return mGender;
	}

	CoinPurse *Character::getCoinPurse() const
	{
		return mCoinPurse;
	}

	data::IData *Character::getSaveObject()
	{
		data::IData *obj_output = GameObject::getSaveObject();
		data::Map *output = dynamic_cast<data::Map *>(obj_output);
		if (!output)
		{
			am_log("ERROR", "Save game object from GameObject not a data::Map!");
			return NULL;
		}

		output->push("moveX", mMoveX);
		output->push("moveY", mMoveY);

		output->push("pickupReach", mPickupReach);
		output->push("age", mAge);
		if (mRace)
		{
			output->push("race", mRace->getRaceName());
		}
		output->push("gender", Gender::getGenderName(mGender));

		data::Map *bodyParts = new data::Map();
		for (auto iter = mBodyParts.begin(); iter != mBodyParts.end(); ++iter)
		{
			bodyParts->push(iter->first, iter->second->getSaveObject());
		}
		output->push("bodyParts", bodyParts);

		if (mInventory)
		{
			output->push("inventory", mInventory->getSaveObject());
		}

		if (mGraphic)
		{
			output->push("graphic", mGraphic->getSaveObject());
		}
		return output;
	}

	void Character::_equipItem(Item *item, const char *bodyPartName)
	{
		item->setItemLocation(Item::INVENTORY);
		mStats.addModifiers(item->getStatModifiers());
		fireEvent<EquipEvent>(new EquipEvent("equip", this, bodyPartName, item));
	}
	void Character::_unequipItem(Item *item, const char *bodyPartName)
	{
		mStats.removeModifiers(item->getStatModifiers());
		fireEvent<EquipEvent>(new EquipEvent("unequip", this, bodyPartName, item));
	}

	void Character::onLevelUp()
	{
		Handle<Event> e(new Event("level_change"));
		fireEvent<Event>(e);
	}
	void Character::onExperienceChange()
	{
		Handle<Event> e(new Event("experience_change"));
		fireEvent<Event>(e);
	}
}
}
