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
using namespace am::util;

#include <math/math.h>

#include <sstream>
using namespace std;

#include <log/logger.h>

#include <gl.h>

#include "engine.h"

namespace am {
namespace game {

	const int Character::LUA_ID = 0x04;
	const char *Character::LUA_TABLENAME = "am_game_Character";

	Character::Character() :
		GameObject(),
		Levelable(),
		mGraphic(NULL),
		mController(new AiController()),
		mAge(1.0f),
		mGender(Gender::MALE),
		mRace(NULL),
		mCoinPurse(new CoinPurse()),
		mStats(new Stats())
	{
		mFixedToGrid = true;
		setName("Character");
		mPickupReach = Engine::getEngine()->getGridSize() * 1.5f;
		mMaxLevel = 50;
		addEventListener(MOUSE_UP, this);

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
		mCoinPurse(new CoinPurse(*copy.mCoinPurse))
	{
		// Copy BodyParts, Inventory, Stats
		if (copy.mGraphic)
		{
			setGraphic(new Sprite(*copy.mGraphic), false);
		}
		mStats = new Stats(*copy.mStats);
		mStats->setAttachedTo(this);

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
		
	}
	bool Character::onGameTick(float dt)
	{
		if (mController)
		{
			mController->update(this, dt);
		}
		Handle<IAction> currentAction(getCurrentAction());
		if (currentAction)
		{
			if (!currentAction->update(this, dt))
			{
				mActions.erase(mActions.begin());
			}
		}
		if (!mDestinationPath.empty() && mDestinationPos >= 0 && mDestinationPos < mDestinationPath.size())
		{
			float timeTaken = 0.0f;
			Vector2f pos(mLocationX, mLocationY);
			while (timeTaken < dt)
			{
				const Vector2f &dest(mDestinationPath[mDestinationPos]);
				Vector2f toDest(dest.sub(pos));
				float length = toDest.length<float>();
				if (length < 1.0f)
				{
					setLocation(dest.x, dest.y);
					mDestinationPos++;
					if (mDestinationPos >= mDestinationPath.size())
					{
						mDestinationPath.clear();
						mDestinationPos = 0;
						break;
					}
					continue;
				}
				Vector2f toDestSpeed(toDest);
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
	Vector2f Character::getDestination() const
	{
		return mDestination;
	}
	Vector2i Character::getGridDestination() const
	{
		return Engine::getEngine()->worldToGrid(mDestination);
	}
	float Character::getDestinationLength() const
	{
		if (mDestinationPath.empty() || mDestinationPos >= mDestinationPath.size())
		{
			return 0.0f;
		}
		const Vector2f &destPos(mDestinationPath[mDestinationPos]);
		Vector2f toDest(destPos.sub(Vector2f(mLocationX, mLocationY)));
		float length = toDest.length();
		for (int index = mDestinationPos + 1; index < mDestinationPath.size(); index++)
		{
			Vector2f diff = mDestinationPath[index].sub(mDestinationPath[index + 1]);
			length += diff.length();
		}
		return length;
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
		Handle<IAction> current(getCurrentAction());
		if (current)
		{
			current->stop();
		}
	}
	IAction *Character::getCurrentAction() const
	{
		if (mActions.empty())
		{
			return NULL;
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

		mDestination = Vector2f(x, y);
		recalcDestination();
	}
	void Character::recalcDestination()
	{
		Vector2i gridDest = Engine::getEngine()->worldToGrid(mDestination);

		mDestinationPath.clear();
		mDestinationPos = 0;
		mMap->search(Vector2i(getGridLocationX(), getGridLocationY()), 
			gridDest, mDestinationPath, this);

		if (!mDestinationPath.empty())
		{
			Vector2i endGrid = Engine::getEngine()->worldToGrid(mDestinationPath.back());
			Vector2i diff = endGrid.sub(gridDest);
			if (diff.x != 0 || diff.y != 0)
			{
				// Move to the little bit at the end.
				mDestinationPath.back() = mDestination;
			}
		}
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
		mDestination = Engine::getEngine()->gridToWorld(Vector2i(x, y));
		mDestinationPos = 0;
		mMap->search(Vector2i(getGridLocationX(), getGridLocationY()), 
			Vector2i(x, y), mDestinationPath, this);
	}
	bool Character::hasDestination() const
	{
		return !mDestinationPath.empty();
	}

	float Character::getSpeed()
	{
		return mStats->getStat(Stat::SPEED);
	}

	data::IData *Character::serialise()
	{
		data::IData *obj_output = GameObject::serialise();
		data::Table *output = dynamic_cast<data::Table *>(obj_output);
		if (!output)
		{
			am_log("ERROR", "Save game object from GameObject not a data::Map!");
			return NULL;
		}

		output->at("pickupReach", mPickupReach);
		output->at("age", mAge);
		if (mRace)
		{
			output->at("race", mRace->getRaceName());
		}
		output->at("gender", Gender::getGenderName(mGender));

		data::Table *bodyParts = new data::Table();
		for (auto iter = mBodyParts.begin(); iter != mBodyParts.end(); ++iter)
		{
			bodyParts->at(iter->first, iter->second->serialise());
		}
		output->at("bodyParts", bodyParts);

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

		Handle<data::Table> dataMap(dynamic_cast<data::Table *>(data));
		if (!dataMap)
		{	// Shouldn't happen due to GameObject::deserialise
			return -1;
		}

		Handle<data::Number> num(dataMap->at<data::Number>("pickupReach"));
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
		Handle<data::Number> num2(dataMap->at<data::Number>("destinationY"));
		if (num && num2)
		{
			// The destination path will be calculated in hte post-loading stage
			// once we have a map with all its data loaded.
			mDestination.x = num->value<float>();
			mDestination.y = num2->value<float>();
			state->setGameObjHasDestination(mGameId.c_str(), true);
		}

		Handle<data::String> str(dataMap->at<data::String>("race"));
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
				stringstream ss;
				ss << "Unknown gender '" << str->string() << '\'';
				am_log("LOADERR", ss);
			}
		}

		Handle<data::Table> bodyParts(dataMap->at<data::Table>("bodyParts"));
		if (bodyParts)
		{
			for (auto iter = bodyParts->beginMap(); iter != bodyParts->endMap(); ++iter)
			{
				BodyPart *part = new BodyPart(iter->first.c_str());
				if (!part->deserialise(state, iter->second.get()))
				{
					delete part;
					continue;
				}
				addBodyPart(part);
			}
		}

		Handle<data::IData> tempData(dataMap->at("inventory"));
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
			Sprite *graphic = new Sprite();
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

		return 1;
	}

	void Character::_equipItem(Item *item, const char *bodyPartName)
	{
		item->setItemLocation(Item::INVENTORY);
		mStats->addModifiers(item->getStatModifiers());
		fireEvent<EquipEvent>(new EquipEvent("equip", this, bodyPartName, item));
	}
	void Character::_unequipItem(Item *item, const char *bodyPartName)
	{
		mStats->removeModifiers(item->getStatModifiers());
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
		}
		GameObject::postRender(dt);
	}
}
}
