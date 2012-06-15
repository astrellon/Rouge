#pragma once

#include "ievent_listener.h"
#include "ui_component.h"

#include <string>

#include "handle.h"

using namespace std;

namespace am {
namespace gfx {
	class Asset;
	class TextField;
	class Renderable;
	class Sprite;
}

using namespace am::gfx;

namespace ui {

	class Button : public IEventListener, public UIComponent {
	public:
		Button();
		Button(const char *assetName, Renderable *hitbox = NULL);
		Button(Asset *asset, Renderable *hitbox = NULL);
		Button(const char *assetName, const char *label, Renderable *hitbox = NULL);
		Button(Asset *asset, const char *label, Renderable *hitbox = NULL);
		~Button();

		void setGraphic(Sprite *graphic);
		Sprite *getGraphic();

		void setHitbox(Renderable *hitbox);
		Renderable *getHitbox();

		void setLabel(const char *label);
		void setLabel(const string &label);
		string getLabel() const;

		TextField *getLabelField();

		virtual void onEvent(MouseEvent *e);

		virtual void setWidth(float width);
		virtual float getWidth();

		virtual void setHeight(float height);
		virtual float getHeight();

		virtual void setEnabled(bool enabled);

	protected:

		am::util::Handle<Renderable> mHitbox;
		am::util::Handle<Sprite> mGraphic;
		am::util::Handle<TextField> mLabel;

		void init();
		void addListeners(Renderable *target);
		void removeListeners(Renderable *target);

		void updateLabelPos();
	};

}
}
