#pragma once

#include <base/handle.h>
#include <base/return_codes.h>
using namespace am::base;

#include <string>
#include <vector>
using namespace std;

#include <gfx/gfx_sprite.h>
using namespace am::gfx;

#include <ui/ievent_listener.h>
using namespace am::ui;

#include "game_object.h"

namespace am {
namespace game {

	class Door : public GameObject {
	public:

		Door();
		Door(const Door &copy);
		~Door();

		void setGraphic(Sprite *graphic, bool calcCameraOffset = true);
		Sprite *getGraphic() const;
		
		virtual void update(float dt);
		virtual bool onGameTick(float dt);

		virtual float getWidth();
		virtual float getHeight();

		virtual void receiveDamage(float damage);

		virtual void setOpened(bool opened);
		virtual bool isOpened() const;
		
		virtual const char *getGameObjectTypeName() const;
		virtual GameObjectType getGameObjectType() const;

		virtual data::IData *serialise();
		virtual int deserialise(LoadingState *state, data::IData *data);

		virtual void updateGraphic();

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		// TODO: Probably need to be a vector, or even a different class that
		// keeps track of multiple animations.
		Handle<Sprite> mGraphic;

		bool mOpened;
	};

}
}
