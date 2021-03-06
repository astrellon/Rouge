#include "character.h"

#include "engine.h"
#include "game.h"
#include "map.h"
#include "race.h"
#include "ai_controller.h"

#include <ui/equip_event.h>

#include <util/data_table.h>
#include <util/data_number.h>
#include <util/data_boolean.h>
#include <util/data_string.h>
#include <util/utils.h>

#include <math/math.h>

#include <sstream>

#include <log/logger.h>

#include <gl.h>
#include <gfx/gfx_texture.h>

#include "engine.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	using namespace am::util;

	const int Character::LUA_ID = LUA_ID_CHARACTER;
	const char *Character::LUA_TABLENAME = LUA_TABLE_CHARACTER;

	Character::Character() :
		GameObject(),
		Levelable(),
		mGraphic(nullptr),
		mController(new AiController()),
		mAge(1.0f),
		mGender(Gender::MALE),
		mRace(nullptr),
		mCoinPurse(new CoinPurse()),
		mStats(new Stats()),
		mArmedCounter(0),
		mBackground(new gfx::Layer()),
		mForeground(new gfx::Layer()),
		mCharacterLayer(new gfx::Layer()),
		mAIFuncRef(LUA_REFNIL),
		mDestinationPos(-1)
	{
		mBackground->setName("Character->Background");
		mForeground->setName("Character->Foreground");
		mCharacterLayer->setName("Character->CharacterLayer");
		mCharacterLayer->setInteractive(true);
		mFixedToGrid = true;
		setName("Character");
		mPickupReach = Engine::getEngine()->getGridSize() * 1.5f;
		mMaxLevel = 50;

		addChild(mBackground);
		addChild(mCharacterLayer);
		addChild(mForeground);

		mCoinPurse->addEventListener("coin_change", this);

		mInventory = new Inventory(10, 6);
		mStats->setAttachedTo(this);
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
		mInventory(new Inventory(*copy.mInventory)),
		mArmedCounter(0),
		mAIFuncRef(copy.mAIFuncRef),
		mDestinationPos(-1)
	{
		int numChildren = getNumChildren();
		if (numChildren >= 1)
		{
			mBackground = dynamic_cast<gfx::Layer *>(getChildAt(0));
		}
		else
		{
			mBackground = new gfx::Layer();
			mBackground->setName("Character->Background");
			addChild(mBackground, 0);
		}
		if (numChildren >= 2)
		{
			mCharacterLayer = dynamic_cast<gfx::Layer *>(getChildAt(1));
		}
		else
		{
			mCharacterLayer = new gfx::Layer();
			mCharacterLayer->setInteractive(true);
			mCharacterLayer->setName("Character->CharacterLayer");
			addChild(mCharacterLayer, 1);
		}
		if (numChildren >= 3)
		{
			mForeground = dynamic_cast<gfx::Layer *>(getChildAt(2));
		}
		else
		{
			mForeground = new gfx::Layer();
			mForeground->setName("Character->Foreground");
			addChild(mForeground, 2);
		}

		// Copy BodyParts, Inventory, Stats
		if (copy.mGraphic)
		{
			// Find our new graphic in the character layer if there was one.
			for (int i = 0; i < mCharacterLayer->getNumChildren(); i++)
			{
				gfx::Sprite *temp = dynamic_cast<gfx::Sprite *>(mCharacterLayer->getChildAt(i));
				if (temp && temp->getAsset() == copy.mGraphic->getAsset())
				{
					mGraphic = temp;
				}
			}
		}

		mStats = new Stats(*copy.mStats);
		// Calls updateGraphic internally, so it must be done after we've
		// grabbed our graphic and various layers.
		mStats->setAttachedTo(this);

		const BodyParts::PartList &partList = copy.getBodyParts().getAllParts();
		for (auto iter = partList.begin(); iter != partList.end(); ++iter)
		{
			addBodyPart(new BodyPart(*iter->get()));
		}

		mCoinPurse->addEventListener("coin_change", this);
	}
	Character::~Character()
	{
		if (mController)
		{
			mController->detach();
		}
		if (mAIFuncRef != LUA_REFNIL)
		{
			luaL_unref(Engine::getEngine()->getLua(), LUA_REGISTRYINDEX, mAIFuncRef);
            mAIFuncRef = LUA_REFNIL;
		}
	}

	void Character::setGraphic(gfx::Sprite *graphic, bool calcCameraOffset)
	{
		base::Handle<gfx::Sprite> currentGraphic(mGraphic);
		if (mGraphic)
		{
			mCharacterLayer->removeChild(mGraphic);
		}
		mGraphic = graphic;
		if (graphic)
		{
			graphic->setInteractive(true);
		}
		if (graphic && calcCameraOffset)
		{
			// Aim for head-ish area.
			mCameraOffsetX = graphic->getWidth() * 0.5f;
			mCameraOffsetY = graphic->getHeight() * 0.3f;
		}
		updateGraphic();
	}
	gfx::Sprite *Character::getGraphic() const
	{
		return mGraphic;
	}

	void Character::setDeadGraphic(gfx::Sprite *graphic)
	{
		mDeadGraphic = graphic;
	}
	gfx::Sprite *Character::getDeadGraphic() const
	{
		return mDeadGraphic;
	}

	gfx::Layer *Character::getBackgroundLayer() const
	{
		return mBackground;
	}
	gfx::Layer *Character::getForegroundLayer() const
	{
		return mForeground;
	}
	gfx::Layer *Character::getCharacterLayer() const
	{
		return mCharacterLayer;
	}
		   
	void Character::update(float dt)
	{
		if (mController)
		{
			mController->update(this, dt);
		}
	}
	bool Character::onGameTick(float dt)
	{
		if (isDead())
		{
			// Life doesn't wait for the dead *sunglasses*
			// In that returning true means that this characters game tick turn is done.
			return true;
		}
		
		if (mController)
		{
			mController->onGameTick(this, dt);
		}

		base::Handle<IAction> currentAction(getCurrentAction());
		if (currentAction)
		{
			if (!currentAction->update(this, dt))
			{
				mActions.erase(mActions.begin());
			}
		}
		if (!mDestinationPath.empty() && mDestinationPos >= 0 && mDestinationPos < static_cast<int>(mDestinationPath.size()))
		{
			float timeTaken = 0.0f;
			math::Vector2f pos(mLocationX, mLocationY);
			while (timeTaken < dt && !mDestinationPath.empty())
			{
				const math::Vector2f &dest(mDestinationPath[mDestinationPos]);
				math::Vector2f toDest(dest.sub(pos));
				float length = toDest.length<float>();
				if (length < 1.0f)
				{
					setLocation(dest.x, dest.y);
					mDestinationPos++;
					if (mDestinationPos >= static_cast<int>(mDestinationPath.size()))
					{
						clearDestination();
						break;
					}
					continue;
				}
				math::Vector2f toDestSpeed(toDest);
				float speed = getSpeed() * Engine::gridSize();
				float distTime = length / speed;
				if (distTime > dt - timeTaken)
				{
					distTime = dt - timeTaken;
				}
				toDestSpeed.normalise();
				toDestSpeed = toDestSpeed.scale(speed * distTime);

				move(toDestSpeed.x, toDestSpeed.y);

				timeTaken += distTime;
				pos.x = mLocationX;
				pos.y = mLocationY;
			}
		}

		if (!mController)
		{
			return true;
		}
		return false;
	}
	math::Vector2f Character::getDestination() const
	{
		return mDestination;
	}
	math::Vector2i Character::getGridDestination() const
	{
		return Engine::getEngine()->worldToGrid(mDestination);
	}
	float Character::getDestinationLength() const
	{
		if (mDestinationPath.empty() || mDestinationPos >= static_cast<int>(mDestinationPath.size()))
		{
			return 0.0f;
		}
		const math::Vector2f &destPos(mDestinationPath[mDestinationPos]);
		math::Vector2f toDest(destPos.sub(Vector2f(mLocationX, mLocationY)));
		float length = static_cast<float>(toDest.length());
		for (size_t index = mDestinationPos + 1; index < mDestinationPath.size(); index++)
		{
			math::Vector2f diff = mDestinationPath[index].sub(mDestinationPath[index + 1]);
			length += diff.length();
		}
		return length;
	}

	base::ReturnCode Character::attack(GameObject *enemy, Item *withItem)
	{
		if (!enemy)
		{
			return base::NULL_PARAMETER;
		}

		bool looped = false;
		BodyPart *weaponPart = getNextWeaponPart(looped);
		if (!weaponPart)
		{
			// Sad times, no way to attack.
			return base::NO_WEAPON_PART;
		}

		// TODO Determine the max distance for a weapon based on the
		// equipped item/body part.
		float dist = distanceToGrid(enemy);
		if (dist > 1.5f)
		{
			return base::OUT_OF_RANGE;
		}

		Stats temp = *mStats;
		base::Handle<Item> attackItem(weaponPart->getEquippedItem());
		if (attackItem)
		{
			// Armed attack
			temp.addModifiers(attackItem->getStatModifiersSelf());
		}
		else
		{
			// Unarmed attack
		}

		float minDamage = temp.getStat(Stat::MIN_DAMAGE);
		float maxDamage = temp.getStat(Stat::MAX_DAMAGE);
		
		float attackDamage = Utils::randf(minDamage, maxDamage);
		std::stringstream ss;
		ss << "Attack damage: " << minDamage << " - " << maxDamage << " (" << attackDamage << ")";
		am_log("ATTK", ss);

		enemy->receiveDamage(attackDamage);

		if (looped)
		{
			// We've attacked with all our weapons this 'round'.
		}

		return base::SUCCESS;
	}
	void Character::receiveDamage(float damage)
	{
		bool wasDead = isDead();
		mStats->setBaseStat(Stat::HEALTH, mStats->getBaseStat(Stat::HEALTH) - damage);

		if (!wasDead && isDead())
		{
			// Dead D:
			setDead();
		}
	}
	void Character::setDead()
	{
		updateGraphic();
	}

	void Character::setController(IController *controller)
	{
		mController = controller;
	}
	IController *Character::getController()
	{
		return mController;
	}
	
	void Character::setPickupReach(float reach)
	{
		if (reach < 0.0f) 
		{
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
		if (mGraphic)
		{
			return mGraphic->getWidth();
		}
		return 0.0f;
	}
	float Character::getHeight()
	{
		if (mGraphic)
		{
			return mGraphic->getHeight();
		}
		return 0.0f;
	}

	Stats *Character::getStats()
	{
		return mStats;
	}
	const Stats *Character::getStats() const
	{
		return mStats;
	}

	bool Character::addBodyPart(BodyPart *part)
	{
		if (part == nullptr)
		{
			return false;
		}
		const char *name = part->getName();
		if (!mBodyParts.hasBodyPart(name))
		{
			Item *equipped = part->getEquippedItem();
			if (equipped != nullptr)
			{
				_equipItem(equipped, part);
			}
			mBodyParts.addBodyPart(part);
			return true;
		}
		std::stringstream ss;
		ss << "Body part '" << name << "' already on character '" << getName() << "'";
		am_log("CHAR", ss);
		return false;
	}
	bool Character::removeBodyPart(BodyPart *part)
	{
		if (part == nullptr)
		{
			return false;
		}
		return Character::removeBodyPart(part->getName());
	}
	bool Character::removeBodyPart(const char *partName)
	{
		if (partName == nullptr)
		{
			return false;
		}
		base::Handle<BodyPart> part = mBodyParts.getBodyPart(partName);
		if (part != nullptr)
		{
			Item *equipped = part->getEquippedItem();
			if (equipped != nullptr)
			{
				_unequipItem(equipped, part);
			}
			mBodyParts.removeBodyPart(partName);
			return true;
		}
		return false;
	}
	bool Character::hasBodyPart(BodyPart *part) const
	{
		return mBodyParts.hasBodyPart(part);
	}
	bool Character::hasBodyPart(const char *partName) const
	{
		return mBodyParts.hasBodyPart(partName);
	}
	const BodyParts &Character::getBodyParts() const
	{
		return mBodyParts;
	}
	
	bool Character::equipItem(Item *item, const char *partName)
	{
		if (!item || !partName || partName[0] == '\0')
		{
			return false;
		}
		BodyPart *part = mBodyParts.getBodyPart(partName);
		if (part == nullptr)
		{
			std::stringstream ss;
			ss << "Cannot equip item '" << item->getFullItemName() << "' on to '";
			ss << getName() << "' because they do not have a '" << partName << "'";
			am_log("CHAR", ss);
			return false;
		}
		return equipItem(item, part);
	}
	bool Character::equipItem(Item *item, BodyPart *part)
	{
		if (!item || !part)
		{
			return false;
		}

		//Item *alreadyEquipped = part->getEquippedItem();
		int canResult = canEquipItem(item, part);
		if (canResult)
		{
			part->setEquippedItem(item);
			_equipItem(item, part);
			if (item->getBodyPartsRequired() > 1)
			{
				BodyParts::PartList linked;
				mBodyParts.getLinkedParts(part, linked);
				for (size_t i = 0; i < linked.size(); i++)
				{
					linked[i]->setIsHoldingOnto(true);
				}
			}
			return true;
		}
		return false;
	}
	bool Character::unequipItem(const char *bodyPart)
	{
		if (bodyPart == nullptr || bodyPart[0] == '\0')
		{
			return false;
		}
		BodyPart *part = mBodyParts.getBodyPart(bodyPart);
		if (part == nullptr)
		{
			return false;
		}
		Item *equipped = part->getEquippedItem();
		if (equipped != nullptr)
		{
			_unequipItem(equipped, part);
		}
		part->setEquippedItem(nullptr);
		return true;
	}
	base::ReturnCode Character::canEquipItem(Item *item, const char *partName) const
	{
		if (!item || !partName || partName[0] == '\0')
		{
			return base::NULL_PARAMETER;
		}
		BodyPart *part = mBodyParts.getBodyPart(partName);
		if (part == nullptr)
		{
			std::stringstream ss;
			ss << "Cannot check for can equip item '" << item->getFullItemName() << "' on to '";
			ss << getName() << "' because they do not have a '" << partName << "'";
			am_log("CHAR", ss);
			return base::BODY_PART_NOT_FOUND;
		}
		return canEquipItem(item, part);
	}
	base::ReturnCode Character::canEquipItem(Item *item, BodyPart *part) const
	{
		// Can't deal with null for checks.
		if (!item || !part)
		{
			return base::NULL_PARAMETER;
		}

		// Is this a body part that's on this character?
		if (!mBodyParts.hasBodyPart(part))
		{
			return base::BODY_PART_NOT_FOUND;
		}

		// Can the body part be used to equip the given item?
		if (!part->canEquipItem(item))
		{
			return base::BODY_PART_TYPE_MISMATCH;
		}

		unsigned int bodyPartsRequired = item->getBodyPartsRequired();
		base::ReturnCode result = base::ABLE_TO_EQUIP;
		// Item requires multiple body parts to be equipped.
		if (bodyPartsRequired > 1u)
		{
			BodyParts::PartList linked;
			if (!mBodyParts.getLinkedParts(part, linked))
			{
				// There's been an error so it's unlikely that we should allow equipping at this point.
				return base::INTERNAL_ERROR;
			}

			unsigned int bodyPartsAvailable = linked.size();
			// There are not enough body parts that can be used to equip the item.
			if (bodyPartsAvailable < bodyPartsRequired - 1)
			{
				return base::NOT_ENOUGH_BODY_PARTS;
			}
			// Loop through body parts that can be used to equip an
			// item for the main given body part.
			
			for (size_t i = 0; i < linked.size(); i++)
			{
				// This linked body part cannot equip the item
				if (!linked[i]->canEquipItem(item))
				{
					// If this means that there's not enough available
					// linked body parts in total then the item cannot be equipped.
					if (bodyPartsAvailable < bodyPartsRequired - 1)
					{
						return base::NOT_ENOUGH_BODY_PARTS;
					}
					// Seems that we still have enough body parts and perhaps
					// they can be used to equip the item.
					else
					{
						bodyPartsAvailable--;
					}
				}
				// The linked part is able to equip the item, but it is currently
				// has another item equipped or is holding another item.
				if (linked[i]->getEquippedItem() || linked[i]->isHoldingOnto())
				{
					result = base::CAN_EQUIP;
				}
			}
		}
		// Only one part is required and we already know at this point that the main
		// part is capable of equipping the item. So check if it's already in use
		else if (part->getEquippedItem() || part->isHoldingOnto())
		{
			result = base::CAN_EQUIP;
		}

		return result;
	}
	Item *Character::getEquipped(const char *bodyPart) const
	{
		if (bodyPart == nullptr || bodyPart[0] == '\0')
		{
			return nullptr;
		}
		BodyPart *part = mBodyParts.getBodyPart(bodyPart);
		if (part == nullptr)
		{
			return nullptr;
		}
		return part->getEquippedItem();
	}

	Inventory *Character::getInventory()
	{
		return mInventory;
	}

	base::ReturnCode Character::pickupItem(Item *item)
	{
		if (item == nullptr)
		{
			return base::NULL_PARAMETER;
		}
		if (item->getItemLocation() == Item::GROUND)
		{
			if (!canReachLocation(getLocationX(), getLocationY()))
			{
				return base::OUT_OF_RANGE;
			}
		}
		// There may not be space so this can still return false.
		if (addItem(item))
		{
			base::Handle<ui::ItemEvent> e(new ui::ItemEvent("item_pickedup", item, this));
			fireEvent<ui::ItemEvent>(e);
			item->pickedUp(this);
			return base::SUCCESS;
		}
		else
		{
			return base::NOT_ENOUGH_INVENTORY_SPACE;
		}
	}
	bool Character::addItem(Item *item)
	{
		if (item == nullptr)
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
		if (item == nullptr)
		{
			return false;
		}

		return mInventory->removeItem(item);
	}
	bool Character::hasItem(const Item *item) const
	{
		if (item == nullptr)
		{
			return false;
		}

		return mInventory->hasItem(item);
	}
	base::ReturnCode Character::dropItem(Item *item)
	{
		return dropItem(item, getLocationX(), getLocationY());
	}
	base::ReturnCode Character::dropItem(Item *item, float x, float y)
	{
		if (item == nullptr || mMap == nullptr ||
			x < 0 || y < 0 ||
			x >= mMap->getWidth() ||
			y >= mMap->getHeight())
		{
			am_log("CHAR", "Off map");
			return base::OFF_THE_MAP;
		}

		if (!canReachLocation(x, y))
		{
			return base::OUT_OF_RANGE;
		}

		if (!mMap->isValidLocation(x, y, item))
		{
			am_log("CHAR", "Invalid location for item");
			return base::INVALID_LOCATION;
		}
		item->setLocation(x, y);
		mMap->addGameObject(item);
		item->setItemLocation(Item::GROUND);

		mInventory->removeItem(item);
		base::Handle<ui::ItemEvent> e(new ui::ItemEvent("item_dropped", item, this));
		fireEvent<ui::ItemEvent>(e);
		item->dropped(this);
		return base::SUCCESS;
	}
	bool Character::canReachGameObject(const GameObject *obj) const
	{
		float dx = getLocationX() - obj->getLocationX();
		float dy = getLocationY() - obj->getLocationY();
		if (mPickupReach > 0.0f && (dx < -mPickupReach || dx > mPickupReach || dy < -mPickupReach || dy > mPickupReach))
		{
			return false;
		}
		return true;
	}
	bool Character::canReachLocation(float x, float y) const
	{
		float dx = getLocationX() - x;
		float dy = getLocationY() - y;
		if (mPickupReach > 0.0f && (dx < -mPickupReach || dx > mPickupReach || dy < -mPickupReach || dy > mPickupReach))
		{
			return false;
		}
		return true;
	}

    base::ReturnCode Character::interactWith(GameObject *interacter, bool byMovement)
	{
        base::ReturnCode result = GameObject::interactWith(interacter, byMovement);
        if (result != base::DID_NOT_INTERACT)
        {
            return result;
        }
        return interactDialogue(interacter, byMovement);
	}

	const char *Character::getGameObjectTypeName() const
	{
		return "character";
	}
	GameObject::GameObjectType Character::getGameObjectType() const
	{
		return CHARACTER;
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

	void Character::addAction(IAction *action)
	{
		if (action && findAction(action) == -1)
		{
			mActions.push_back(action);
		}
	}
	void Character::removeAction(IAction *action)
	{
		if (action)
		{
			size_t index = findAction(action);
			if (index != -1)
			{
				if (action == getCurrentAction())
				{
					stopCurrentAction();
				}
				mActions.erase(mActions.begin() + index);
			}
		}
	}
	void Character::removeAllActions()
	{
		if (!mActions.empty())
		{
			stopCurrentAction();
			mActions.clear();
		}
	}
	const Character::ActionQueue &Character::getActionQueue() const
	{
		return mActions;
	}
	void Character::stopCurrentAction()
	{
		base::Handle<IAction> current(getCurrentAction());
		if (current)
		{
			current->stop();
		}
	}
	IAction *Character::getCurrentAction() const
	{
		if (mActions.empty())
		{
			return nullptr;
		}
		return mActions[0];
	}

	void Character::setDestination(float x, float y)
	{
		if (!mMap)
		{
			return;
		}
		if (x < 0 || x >= static_cast<float>(mMap->getMapWidth()) * Engine::gridSize() ||
			y < 0 || y >= static_cast<float>(mMap->getMapHeight()) * Engine::gridSize())
		{
			return;
		}

		mDestination = math::Vector2f(x, y);
		recalcDestination();
	}
	void Character::recalcDestination()
	{
		math::Vector2i gridDest = Engine::getEngine()->worldToGrid(mDestination);

		mDestinationPath.clear();
		mDestinationPos = 0;
		mMap->search(math::Vector2i(getGridLocationX(), getGridLocationY()), 
			gridDest, mDestinationPath, this);

		// Not as useful as first thought, might be useful for characters that aren't fixed to the grid.
		/*if (!mDestinationPath.empty())
		{
			Vector2i endGrid = Engine::getEngine()->worldToGrid(mDestinationPath.back());
			Vector2i diff = endGrid.sub(gridDest);
			if (diff.x == 0 || diff.y == 0 && mMap->isValidLocation(mDestination.x, mDestination.y, this))
			{
				// Move to the little bit at the end.
				mDestinationPath.back() = mDestination;
			}
		}*/
	}
	void Character::clearDestination()
	{
		mDestinationPath.clear();
		mDestinationPos = -1;

		mDestination.x = mLocationX;
		mDestination.y = mLocationY;
	}
	void Character::setGridDestination(int x, int y)
	{
		if (!mMap)
		{
			return;
		}
		if (x < 0 || x >= mMap->getMapWidth() ||
			y < 0 || y >= mMap->getMapHeight())
		{
			return;
		}

		mDestinationPath.clear();
		mDestination = Engine::getEngine()->gridToWorld(math::Vector2i(x, y));
		mDestinationPos = 0;
		mMap->search(math::Vector2i(getGridLocationX(), getGridLocationY()), 
			math::Vector2i(x, y), mDestinationPath, this);
	}
	bool Character::hasDestination() const
	{
		return !mDestinationPath.empty();
	}

	void Character::setAIFunc(int funcRef)
	{
		mAIFuncRef = funcRef;
	}
	int Character::getAIFunc() const
	{
		return mAIFuncRef;
	}

	float Character::getSpeed()
	{
		return mStats->getStat(Stat::SPEED);
	}
	bool Character::isDead() const
	{
		return mStats->getStat(Stat::HEALTH) <= 0.0f;
	}

    base::ReturnCode Character::addStore(Store *store)
    {
        if (store == nullptr)
        {
            return base::NULL_PARAMETER;
        }
        if (hasStore(store))
        {
            return base::STORE_ALREADY_OWNED;
        }
        mOwnedStores.push_back(store);
        return base::SUCCESS;
    }
    Store *Character::getStore(const char *name) const
    {
        if (!name || name[0] == '\0')
        {
            return nullptr;
        }
        for (size_t i = 0; i < mOwnedStores.size(); i++)
        {
            if (strcmp(mOwnedStores[i]->getStoreId(), name) == 0)
            {
                return mOwnedStores[i];
            }
        }
        return nullptr;
    }

    Store *Character::getStore(int index) const
    {
        if (index < 0 || index >= static_cast<int>(mOwnedStores.size()))
        {
            return nullptr;
        }
        return mOwnedStores[index];
    }
    bool Character::hasStore(Store *store) const
    {
        if (store == nullptr)
        {
            return false;
        }
        for (auto iter = mOwnedStores.begin(); iter != mOwnedStores.end(); ++iter)
        {
            if (iter->get() == store)
            {
                return true;
            }
        }
        return false;
    }
    const Character::StoreList &Character::getStores() const
    {
        return mOwnedStores;
    }

	data::IData *Character::serialise()
	{
		data::IData *obj_output = GameObject::serialise();
		data::Table *output = dynamic_cast<data::Table *>(obj_output);
		if (!output)
		{
			am_log("ERROR", "Save game object from GameObject not a data::Map!");
			return nullptr;
		}

		output->at("pickupReach", mPickupReach);
		output->at("age", mAge);
		if (mRace)
		{
			output->at("race", mRace->getRaceName());
		}
		output->at("gender", Gender::getGenderName(mGender));

		output->at("bodyParts", mBodyParts.serialise());

		output->at("stats", mStats->serialise());
		if (mInventory)
		{
			output->at("inventory", mInventory->serialise());
		}

		if (mGraphic)
		{
			output->at("graphic", mGraphic->serialise());
		}

		if (mCoinPurse)
		{
			output->at("coinPurse", mCoinPurse->serialise());
		}

		if (hasDestination())
		{
			output->at("destinationX", mDestination.x);
			output->at("destinationY", mDestination.y);
		}

		output->at("levelable", Levelable::serialise());
		return output;
	}
	int Character::deserialise(LoadingState *state, data::IData *data)
	{
		int loadResult = GameObject::deserialise(state, data);
		if (loadResult < 1)
		{
			return loadResult;
		}

		base::Handle<data::Table> dataMap(dynamic_cast<data::Table *>(data));
		if (!dataMap)
		{	// Shouldn't happen due to GameObject::deserialise
			return -1;
		}

		state->pushCurrentCharacter(this);
		base::Handle<data::Number> num(dataMap->at<data::Number>("pickupReach"));
		if (num)
		{
			setPickupReach(num->value<float>());
		}

		num = dataMap->at<data::Number>("age");
		if (num)
		{
			setAge(num->value<float>());
		}

		num = dataMap->at<data::Number>("destinationX");
		base::Handle<data::Number> num2(dataMap->at<data::Number>("destinationY"));
		if (num && num2)
		{
			// The destination path will be calculated in hte post-loading stage
			// once we have a map with all its data loaded.
			mDestination.x = num->value<float>();
			mDestination.y = num2->value<float>();
			state->setGameObjHasDestination(mGameId.c_str(), true);
		}

		base::Handle<data::String> str(dataMap->at<data::String>("race"));
		if (str)
		{
			setRace(Engine::getEngine()->getRace(str->string()));
		}

		str = dataMap->at<data::String>("gender");
		if (str)
		{
			Gender::GenderType gender = Gender::getGenderType(str->string());
			if (gender != Gender::MAX_GENDER_LENGTH)
			{
				setGender(gender);
			}
			else
			{
				std::stringstream ss;
				ss << "Unknown gender '" << str->string() << '\'';
				am_log("LOADERR", ss);
			}
		}

		base::Handle<data::Table> bodyParts(dataMap->at<data::Table>("bodyParts"));
		if (bodyParts)
		{
			if (mBodyParts.deserialise(state, bodyParts))
			{
				const BodyParts::PartList &parts = mBodyParts.getAllParts();
				for (auto iter = parts.begin(); iter != parts.end(); ++iter)
				{
					addBodyPart(iter->get());
				}
			}
		}

		base::Handle<data::IData> tempData(dataMap->at("inventory"));
		if (tempData)
		{
			mInventory->deserialise(state, tempData);
		}

		tempData = dataMap->at("stats");
		if (tempData)
		{
			mStats->deserialise(state, tempData);
		}

		tempData = dataMap->at("graphic");
		if (tempData)
		{
			gfx::Sprite *graphic = new gfx::Sprite();
			graphic->deserialise(state, tempData);
			setGraphic(graphic, false);
		}

		tempData = dataMap->at("levelable");
		if (tempData)
		{
			Levelable::deserialise(state, tempData);
		}

		tempData = dataMap->at("coinPurse");
		if (tempData && mCoinPurse)
		{
			mCoinPurse->deserialise(state, tempData);
		}

		state->popCurrentCharacter();
		return 1;
	}

	void Character::_equipItem(Item *item, BodyPart *part)
	{
		if (!part || !item)
		{
			return;
		}

		if (part->isWeaponPart())
		{
			mArmedCounter++;
		}
		item->setItemLocation(Item::INVENTORY);
		mStats->addModifiers(item->getStatModifiers());
		fireEvent<ui::EquipEvent>(new ui::EquipEvent("equip", this, part, item));
	}
	void Character::_unequipItem(Item *item, BodyPart *part)
	{
		if (!part || !item)
		{
			return;
		}
		if (part->isWeaponPart())
		{
			mArmedCounter--;
		}
		mStats->removeModifiers(item->getStatModifiers());
		fireEvent<ui::EquipEvent>(new ui::EquipEvent("unequip", this, part, item));
	}

	void Character::onLevelUp()
	{
		base::Handle<ui::Event> e(new ui::Event("level_change"));
		fireEvent<ui::Event>(e);
	}
	void Character::onExperienceChange()
	{
		base::Handle<ui::Event> e(new ui::Event("experience_change"));
		fireEvent<ui::Event>(e);
	}

	BodyPart *Character::getNextWeaponPart(bool &looped)
	{
		looped = false;
		size_t len = mBodyParts.getAllParts().size();
		for (size_t i = 0; i < len; i++)
		{
			BodyPart *part = mBodyParts.getNextWeaponPart(looped);
			if (part == nullptr)
			{
				break;
			}
			if (part->getEquippedItem() == nullptr)
			{
				if (mArmedCounter == 0)
				{
					return part;
				}
			}
			else
			{
				return part;
			}
		}
		return nullptr;
	}

	size_t Character::findAction(IAction *action)
	{
		if (!action)
		{
			return -1;
		}
		for (size_t i = 0; i < mActions.size(); i++)
		{
			if (mActions[i].get() == action)
			{
				return i;
			}
		}
		return -1;
	}

	void Character::postRender(float dt)
	{
		if (!mDestinationPath.empty())
		{
			
			glPushMatrix();
			gfx::Texture::bindTexture(0);
			Engine *engine = Engine::getEngine();
			glTranslatef(-mLocationX + engine->getGridSize() * 0.5f, -mLocationY + engine->getGridSize() * 0.5f, 0.0f);
			glBegin(GL_LINES);
				glColor3d(1.0, 0.3, 0.2);
				for (size_t i = 0; i < mDestinationPath.size() - 1; i++)
				{
					glVertex2f(mDestinationPath[i].x, mDestinationPath[i].y);
					glVertex2f(mDestinationPath[i + 1].x, mDestinationPath[i + 1].y);
				}
			glEnd();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(mDestination.x - mLocationX, mDestination.y - mLocationY, 0.0f);
			glBegin(GL_TRIANGLES);
				glColor3d(0.3, 1.0, 0.2);
				glVertex2f(-5.0f, -5.0f);
				glVertex2f(0.0f, 5.0f);
				glVertex2f(5.0f, -5.0f);
			glEnd();
			glPopMatrix();
		}
		GameObject::postRender(dt);
	}

	void Character::updateGraphic()
	{
		mCharacterLayer->clear();
		if (isDead())
		{
			if (mDeadGraphic)
			{
				mCharacterLayer->addChild(mDeadGraphic);
			}
			else
			{
				base::Handle<Game> game(Engine::getGame());
				if (!game)
				{
					std::stringstream ss;
					ss << "Unable to set dead graphic for character '" << mGameId
						<< "' as there is no current game yet to grab the generic dead graphic from.";
					am_log("DEAD", ss);
					return;
				}
				base::Handle<gfx::Sprite> dead(game->getGenericDeadGraphic());
				if (dead)
				{
					mDeadGraphic = new gfx::Sprite(*dead);
					mCharacterLayer->addChild(mDeadGraphic);
				}
				else
				{
					std::stringstream ss;
					ss << "Unable to set dead graphic as character '" << mGameId
						<< "' does not have one and no generic one has: been set.";
					am_log("DEAD", ss);
				}
			}
		}
		else
		{
			mCharacterLayer->addChild(mGraphic);
		}
	}

	void Character::onEvent(ui::Event *e)
	{
		// Pass along coin event.
		if (e->getType() == "coin_change")
		{
			fireEvent<ui::Event>(e);
		}
	}
}
}
