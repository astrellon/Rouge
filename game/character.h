#pragma once

#include <base/handle.h>
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

namespace am {
namespace game {

	class Race;

	class Character : public IEventListener, public GameObject, public Levelable {
	public:

		typedef vector< Handle<IAction> > ActionQueue;
		Character();
		Character(const Character &copy);
		~Character();

		void setGraphic(Sprite *graphic, bool calcCameraOffset = true);
		Sprite *getGraphic();

		virtual void update(float dt);
		virtual bool onGameTick(float dt);

		virtual void setController(IController *controller);
		virtual IController *getController();

		virtual void setMoveVector(int x, int y);
		virtual int getMoveVectorX() const;
		virtual int getMoveVectorY() const;

		virtual void setPickupReach(float reach);
		virtual float getPickupReach() const;

		virtual float getWidth();
		virtual float getHeight();

		virtual Stats &getStats();

		virtual bool addBodyPart(BodyPart *part);
		virtual bool removeBodyPart(BodyPart *part);
		virtual bool removeBodyPart(const char *partName);
		virtual bool hasBodyPart(BodyPart *part) const;
		virtual bool hasBodyPart(const char *partName) const;
		virtual const BodyPart::BodyPartMap &getBodyParts() const;

		virtual bool equipItem(Item *item, const char *bodyPart);
		virtual bool unequipItem(const char *bodyPart);
		virtual Item *getEquipped(const char *bodyPart) const;
		
		virtual Inventory *getInventory();

		virtual int pickupItem(Item *item);
		virtual bool addItem(Item *item);
		virtual bool removeItem(Item *item);
		virtual bool hasItem(const Item *item) const;
		virtual int dropItem(Item *item);
		virtual int dropItem(Item *item, float x, float y);

		virtual const char *getGameObjectTypeName() const;

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

		virtual data::IData *serialise();
		virtual int deserialise(LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		Handle<IController> mController;

		int mMoveX;
		int mMoveY;

		float mPickupReach;

		BodyPart::BodyPartMap mBodyParts;

		// TODO: Probably need to be a vector, or even a different class that
		// keeps track of multiple animations.
		Handle<Sprite> mGraphic;

		Handle<Inventory> mInventory;
		Handle<CoinPurse> mCoinPurse;

		ActionQueue mActions;

		Stats mStats;

		float mAge;
		Race *mRace;
		Gender::GenderType mGender;

		NodePath mDestination;

		virtual void onLevelUp();
		virtual void onExperienceChange();

		size_t findAction(IAction *action);
		void _equipItem(Item *item, const char *bodyPartName);
		void _unequipItem(Item *item, const char *bodyPartName);

		virtual void postRender(float dt);
	};

}
}
