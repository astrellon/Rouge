#pragma once

#include "ui_component.h"

#include "ievent_listener.h"

namespace am {
namespace gfx {
	class GfxEngine;
	class Asset;
	class Sprite;
	class TextField;
}

using namespace am::gfx;

namespace ui {

	class Checkbox : public IEventListener, public UIComponent {
	public:
		Checkbox(GfxEngine *engine);
		Checkbox(GfxEngine *engine, Asset *asset, const char *label);
		Checkbox(GfxEngine *engine, const char *assetName, const char *label);
		~Checkbox();

		void setLabel(const char *label);
		void setLabel(const string &label);
		string getLabel() const;

		TextField *getLabelField();
		Sprite *getCheckGraphic();

		virtual void onEvent(MouseEvent *e);

		bool isSelected() const;
		void setSelected(bool selected);

		virtual float getWidth();
		virtual float getHeight();

		virtual void setEnabled(bool enabled);
	protected:

		Sprite *mCheckGraphic;
		TextField *mLabel;
		Sprite *mHitbox;
		bool mSelected;

		void init();
	};

}
}
