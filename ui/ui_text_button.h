#pragma once

#include <string>
using namespace std;

#include <base/handle.h>
using namespace am::base;

#include "ievent_listener.h"
#include "ui_component.h"

namespace am {
namespace gfx {
	class Button;
	class Asset;
	class TextField;
	class Renderable;
	class Sprite;
}

using namespace am::gfx;

namespace ui {

	class TextButton : public IEventListener, public UIComponent {
	public:
		TextButton();
		TextButton(const char *assetName, Renderable *hitbox = NULL);
		TextButton(const char *assetName, const char *label, Renderable *hitbox = NULL);
		~TextButton();

		void setGraphic(Button *graphic);
		Button *getGraphic();

		void setHitbox(Renderable *hitbox);
		Renderable *getHitbox();

		void setLabel(const char *label);
		void setLabel(const string &label);
		string getLabel() const;

		TextField *getLabelField();

		virtual void setWidth(float width);
		virtual float getWidth();

		virtual void setHeight(float height);
		virtual float getHeight();

		virtual void setEnabled(bool enabled);

		virtual void onEvent(Event *e);

	protected:

		Handle<Button> mGraphic;
		Handle<TextField> mLabel;

		void init();

		void updateLabelPos();
	};

}
}
