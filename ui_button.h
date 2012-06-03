#pragma once

#include "ievent_listener.h"
#include "gfx/gfx_layer.h"

#include <string>

using namespace std;

namespace am {
namespace gfx {
	class GfxEngine;
	class Asset;
	class Renderable;
	class Sprite;
}

using namespace am::gfx;

namespace ui {

	class Button : public IEventListener, public Layer {
	public:
		Button(GfxEngine *engine, Renderable *hitbox = NULL);
		Button(GfxEngine *engine, const char *assetName, Renderable *hitbox = NULL);
		Button(GfxEngine *engine, Asset *asset, Renderable *hitbox = NULL);
		~Button();

		void setGraphic(Sprite *graphic);
		Sprite *getGraphic();

		void setHitbox(Renderable *hitbox);
		Renderable *getHitbox();

		void setLabel(const char *label);
		void setLabel(const string &label);
		string getLabel() const;

		virtual void onEvent(MouseEvent *e);

	protected:

		Renderable *mHitbox;
		Sprite *mGraphic;
		string mLabel;

		void init();
		void addListeners(Renderable *target);
		void removeListeners(Renderable *target);
	};

}
}
