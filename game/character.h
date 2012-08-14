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

#include "stats.h"

namespace am {
namespace game {

	class Character : public IEventListener, public GameObject {
	public:
		Character();
		~Character();

		void setGraphic(Sprite *graphic);
		Sprite *getGraphic();

		virtual void update(float dt);

		virtual void setController(IController *controller);
		virtual IController *getController();

		virtual void setMoveVector(int x, int y);
		virtual int getMoveVectorX() const;
		virtual int getMoveVectorY() const;

		virtual float getWidth();
		virtual float getHeight();

		virtual Stats &getStats();

		virtual Inventory *getInventory();

		virtual bool pickupItem(Item *item);
		virtual bool addItem(Item *item);
		virtual bool removeItem(Item *item);
		virtual bool hasItem(const Item *item) const;
		virtual bool dropItem(Item *item, int gridX, int gridY);

		virtual const char *getGameObjectTypeName() const;

	protected:

		Handle<IController> mController;

		int mMoveX;
		int mMoveY;

		// TODO: Probably need to be a vector, or even a different class that
		// keeps track of multiple animations.
		Handle<Sprite> mGraphic;

		Handle<Inventory> mInventory;

		Stats mStats;
	};

}
}
