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

namespace ui {

	class Checkbox : public IEventListener, public UIComponent {
	public:
		Checkbox();
		Checkbox(gfx::Asset *asset, const char *label);
		Checkbox(const char *assetName, const char *label);
		~Checkbox();

		void setLabel(const char *label);
		void setLabel(const std::string &label);
		std::string getLabel() const;

		gfx::TextField *getLabelField();
		gfx::Sprite *getCheckGraphic();

		virtual void onEvent(MouseEvent *e);

		bool isSelected() const;
		void setSelected(bool selected);

		virtual float getWidth();
		virtual float getHeight();

		virtual void setEnabled(bool enabled);
	protected:

		base::Handle<gfx::Sprite> mCheckGraphic;
		base::Handle<gfx::TextField> mLabel;
		base::Handle<gfx::Sprite> mHitbox;
		bool mSelected;

		void init();
	};

}
}
