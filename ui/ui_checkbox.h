#pragma once

#include <base/handle.h>

#include "ui_component.h"
#include "ievent_listener.h"

namespace am {
namespace gfx {
	class Asset;
	class Sprite;
	class TextField;
}

using namespace am::gfx;

namespace ui {

	class Checkbox : public IEventListener, public UIComponent {
	public:
		Checkbox();
		Checkbox(Asset *asset, const char *label);
		Checkbox(const char *assetName, const char *label);
		~Checkbox();

		void setLabel(const char *label);
		void setLabel(const std::string &label);
		std::string getLabel() const;

		TextField *getLabelField();
		Sprite *getCheckGraphic();

		virtual void onEvent(MouseEvent *e);

		bool isSelected() const;
		void setSelected(bool selected);

		virtual float getWidth();
		virtual float getHeight();

		virtual void setEnabled(bool enabled);
	protected:

		base::Handle<Sprite> mCheckGraphic;
		base::Handle<TextField> mLabel;
		base::Handle<Sprite> mHitbox;
		bool mSelected;

		void init();
	};

}
}
