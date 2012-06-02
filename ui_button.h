#pragma once

#include "ievent_listener.h"
#include "gfx/gfx_sprite.h"

namespace am {
namespace gfx {
	class GfxEngine;
	class Asset;
	class Renderable;
}

using namespace am::gfx;

namespace ui {

	class Button : public IEventListener, public Sprite {
	public:
		Button(GfxEngine *engine, Renderable *hitbox = NULL);
		Button(GfxEngine *engine, const char *assetName, Renderable *hitbox = NULL);
		Button(GfxEngine *engine, Asset *asset, Renderable *hitbox = NULL);
		~Button();

		void setHitbox(Renderable *hitbox);
		Renderable *getHitbox();

		virtual void onEvent(MouseEvent *e);

	protected:

		Renderable *mHitbox;
		Sprite *mGraphic;

		void init();
		void addListeners(Renderable *target);
		void removeListeners(Renderable *target);
	};

}
}
