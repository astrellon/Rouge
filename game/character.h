#pragma once

#include <base/handle.h>
#include <base/return_codes.h>

#include <string>
#include <vector>

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_sprite.h>

#include <ui/ievent_listener.h>

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
#include "store.h"

namespace am {
namespace game {

	class Character : public GameObject, public Levelable 
	{
	public:

		typedef std::vector< base::Handle<IAction> > ActionQueue;
        typedef std::vector< base::Handle<Store> > StoreList;
		Character();
		Character(const Character &copy);
		~Character();

		void setGraphic(gfx::Sprite *graphic, bool calcCameraOffset = true);
		gfx::Sprite *getGraphic() const;

		void setDeadGraphic(gfx::Sprite *graphic);
		gfx::Sprite *getDeadGraphic() const;

		gfx::Layer *getBackgroundLayer() const;
		gfx::Layer *getForegroundLayer() const;
		gfx::Layer *getCharacterLayer() const;

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

		/**
		 * TODO Replace the return values of the equip/unequip item calls with ReturnCodes!.
		 */
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
		virtual base::ReturnCode canEquipItem(Item *item, const char *partName) const;

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
		virtual base::ReturnCode canEquipItem(Item *item, BodyPart *part) const;
		virtual Item *getEquipped(const char *bodyPart) const;
		
		virtual Inventory *getInventory();

		virtual base::ReturnCode pickupItem(Item *item);
		virtual bool addItem(Item *item);
		virtual bool removeItem(Item *item);
		virtual bool hasItem(const Item *item) const;
		virtual base::ReturnCode dropItem(Item *item);
		virtual base::ReturnCode dropItem(Item *item, float x, float y);
		virtual bool canReachGameObject(const GameObject *obj) const;
		virtual bool canReachLocation(float x, float y) const;

		virtual bool interactWith(GameObject *interacter, bool byMovement);

		virtual base::ReturnCode attack(GameObject *enemy, Item *withItem);
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
		virtual math::Vector2f getDestination() const;
		virtual math::Vector2i getGridDestination() const;
		virtual float getDestinationLength() const;
		virtual bool hasDestination() const;
		virtual void recalcDestination();
		virtual void clearDestination();

		virtual void setAIFunc(int funcRef);
		virtual int getAIFunc() const;

		virtual data::IData *serialise();
		virtual int deserialise(LoadingState *state, data::IData *data);

		virtual float getSpeed();
		virtual bool isDead() const;

        virtual base::ReturnCode addStore(Store *store);
        virtual Store *getStore(int index = 0) const;
        virtual bool hasStore(Store *store) const;
        virtual const StoreList &getStores() const;

		virtual void updateGraphic();

		virtual void onEvent(ui::Event *e);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		base::Handle<IController> mController;

		float mPickupReach;

		BodyParts mBodyParts;

		// TODO: Probably need to be a vector, or even a different class that
		// keeps track of multiple animations.
		base::Handle<gfx::Sprite> mGraphic;
		base::Handle<gfx::Sprite> mDeadGraphic;
		base::Handle<gfx::Layer> mCharacterLayer;
		base::Handle<gfx::Layer> mForeground;
		base::Handle<gfx::Layer> mBackground;

		base::Handle<Inventory> mInventory;
		base::Handle<CoinPurse> mCoinPurse;

		ActionQueue mActions;

		base::Handle<Stats> mStats;

		float mAge;
		base::Handle<Race> mRace;
		Gender::GenderType mGender;

		NodePath mDestinationPath;
		math::Vector2f mDestination;
		int mDestinationPos;

		int mArmedCounter;

		int mAIFuncRef;

        StoreList mOwnedStores;

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
