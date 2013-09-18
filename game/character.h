#pragma once

#include <base/handle.h>
#include <base/return_codes.h>
using namespace am::base;

#include <string>
#include <vector>
using namespace std;

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_sprite.h>
using namespace am::gfx;

#include <ui/ievent_listener.h>
using namespace am::ui;

#include "icontroller.h"
#include "game_object.h"
#include "body_part.h"
#include "stats.h"
#include "gender.h"
#include "coin_purse.h"
#include "levelable.h"
#include "iaction.h"
#include "astar_node.h"
#include "race.h"
#include "body_parts.h"

namespace am {
namespace game {

	class Character : public GameObject, public Levelable {
	public:

		typedef vector< Handle<IAction> > ActionQueue;
		Character();
		Character(const Character &copy);
		~Character();

		void setGraphic(Sprite *graphic, bool calcCameraOffset = true);
		Sprite *getGraphic() const;

		void setDeadGraphic(Sprite *graphic);
		Sprite *getDeadGraphic() const;

		Layer *getBackgroundLayer() const;
		Layer *getForegroundLayer() const;
		Layer *getCharacterLayer() const;

		virtual void update(float dt);
		virtual bool onGameTick(float dt);

		virtual void setController(IController *controller);
		virtual IController *getController();

		virtual void setPickupReach(float reach);
		virtual float getPickupReach() const;

		virtual float getWidth();
		virtual float getHeight();

		virtual Stats *getStats();
		virtual const Stats *getStats() const;

		virtual bool addBodyPart(BodyPart *part);
		virtual bool removeBodyPart(BodyPart *part);
		virtual bool removeBodyPart(const char *partName);
		virtual bool hasBodyPart(BodyPart *part) const;
		virtual bool hasBodyPart(const char *partName) const;
		virtual const BodyParts &getBodyParts() const;

		virtual bool equipItem(Item *item, const char *partName);
		virtual bool equipItem(Item *item, BodyPart *part);
		virtual bool unequipItem(const char *bodyPart);
		/**
		 * Returns if the item can be equipped onto the given body part.
		 * The difference between 'can' and 'able is 'can' indicates that the item
		 * can be equipped but currently a body part is in use and so currently
		 * cannot have another item equipped. Able indicates that it can be equipped
		 * and the character is also able to do so at this time.
		 *
		 * Return codes:
		 *  ABLE_TO_EQUIP: Able to be equipped
		 *  CAN_EQUIP: Can be equipped
		 *  NULL_PARAMETER: The item is nullptr or the partName is nullptr or the partName is an empty string.
		 *  BODY_PART_NOT_FOUND: No part with the given partName was found.
		 *  BODY_PART_TYPE_MISMATCH: The item cannot be equipped due to body part type mismatches.
		 *  INTERNAL_ERROR: There was an error getting the list of linked body parts.
		 *  NOT_ENOUGH_BODY_PARTS: For items that require multiple body parts to equip, there were not enough parts (available or not).
		 */
		virtual ReturnCode canEquipItem(Item *item, const char *partName) const;

		/**
		 * Returns if the item can be equipped onto the given body part.
		 * The difference between 'can' and 'able is 'can' indicates that the item
		 * can be equipped but currently a body part is in use and so currently
		 * cannot have another item equipped. Able indicates that it can be equipped
		 * and the character is also able to do so at this time.
		 *
		 * Return codes:
		 *  ABLE_TO_EQUIP: Able to be equipped
		 *  CAN_EQUIP: Can be equipped
		 *  NULL_PARAMETER: The item is nullptr or the partName is nullptr or the partName is an empty string.
		 *  BODY_PART_NOT_FOUND: The given part was not found on the list of character body parts.
		 *  BODY_PART_TYPE_MISMATCH: The item cannot be equipped due to body part type mismatches.
		 *  INTERNAL_ERROR: There was an error getting the list of linked body parts.
		 *  NOT_ENOUGH_BODY_PARTS: For items that require multiple body parts to equip, there were not enough parts (available or not).
		 */
		virtual ReturnCode canEquipItem(Item *item, BodyPart *part) const;
		virtual Item *getEquipped(const char *bodyPart) const;
		
		virtual Inventory *getInventory();

		virtual ReturnCode pickupItem(Item *item);
		virtual bool addItem(Item *item);
		virtual bool removeItem(Item *item);
		virtual bool hasItem(const Item *item) const;
		virtual ReturnCode dropItem(Item *item);
		virtual ReturnCode dropItem(Item *item, float x, float y);
		virtual bool canReachLocation(float x, float y) const;

		virtual ReturnCode attack(GameObject *enemy, Item *withItem);
		virtual void receiveDamage(float damage);
		
		virtual const char *getGameObjectTypeName() const;
		virtual GameObjectType getGameObjectType() const;

		virtual void setAge(float age);
		virtual float getAge() const;

		virtual void setRace(Race *race);
		virtual Race *getRace() const;

		virtual void setGender(Gender::GenderType gender);
		virtual Gender::GenderType getGender() const;

		virtual CoinPurse *getCoinPurse() const;

		virtual void addAction(IAction *action);
		virtual void removeAction(IAction *action);
		virtual void removeAllActions();
		virtual const ActionQueue &getActionQueue() const;
		virtual void stopCurrentAction();
		virtual IAction *getCurrentAction() const;

		virtual void setDestination(float x, float y);
		virtual void setGridDestination(int x, int y);
		virtual Vector2f getDestination() const;
		virtual Vector2i getGridDestination() const;
		virtual float getDestinationLength() const;
		virtual bool hasDestination() const;
		virtual void recalcDestination();

		virtual void setAIFunc(int funcRef);
		virtual int getAIFunc() const;

		virtual data::IData *serialise();
		virtual int deserialise(LoadingState *state, data::IData *data);

		virtual float getSpeed();
		virtual bool isDead() const;

		virtual void updateGraphic();

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		Handle<IController> mController;

		float mPickupReach;

		BodyParts mBodyParts;

		// TODO: Probably need to be a vector, or even a different class that
		// keeps track of multiple animations.
		Handle<Sprite> mGraphic;
		Handle<Sprite> mDeadGraphic;
		Handle<Layer> mCharacterLayer;
		Handle<Layer> mForeground;
		Handle<Layer> mBackground;

		Handle<Inventory> mInventory;
		Handle<CoinPurse> mCoinPurse;

		ActionQueue mActions;

		Handle<Stats> mStats;

		float mAge;
		Handle<Race> mRace;
		Gender::GenderType mGender;

		NodePath mDestinationPath;
		Vector2f mDestination;
		int mDestinationPos;

		int mArmedCounter;

		int mAIFuncRef;

		virtual void onLevelUp();
		virtual void onExperienceChange();

		virtual BodyPart *getNextWeaponPart(bool &looped);

		virtual void setDead();

		size_t findAction(IAction *action);
		void _equipItem(Item *item, BodyPart *part);
		void _unequipItem(Item *item, BodyPart *part);

		virtual void postRender(float dt);
	};

}
}
