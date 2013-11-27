#pragma once

#include <string>

#include <base/handle.h>

#include "ievent_listener.h"
#include "ui_component.h"

#include <gfx/gfx_button.h>

namespace am {
namespace gfx {
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
		TextButton(const char *assetName, Renderable *hitbox = nullptr);
		TextButton(const char *assetName, const char *label, Renderable *hitbox = nullptr);
		TextButton(const TextButton &copy);
		~TextButton();

		void setGraphic(Button *graphic);
		Button *getGraphic();

		void setHitbox(Renderable *hitbox);
		Renderable *getHitbox();

		void setLabel(const char *label);
		void setLabel(const std::string &label);
		std::string getLabel() const;

		TextField *getLabelField() const;

		virtual void setWidth(float width);
		virtual float getWidth();

		virtual void setHeight(float height);
		virtual float getHeight();

		virtual void setEnabled(bool enabled);

		virtual void onEvent(Event *e);
		
		/*virtual void setTooltip(const char *tooltip);
		virtual const char *getTooltip() const;
		virtual void setDetailedTooltip(const char *tooltip);
		virtual const char *getDetailedTooltip() const;*/

	protected:

		base::Handle<Button> mGraphic;
		base::Handle<TextField> mLabel;

		void init();

		void updateLabelPos();
	};

}
}
