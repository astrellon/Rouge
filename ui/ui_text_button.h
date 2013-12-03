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

namespace ui {

	class TextButton : public IEventListener, public UIComponent
	{
	public:
		TextButton();
		TextButton(const char *assetName, gfx::Renderable *hitbox = nullptr);
		TextButton(const char *assetName, const char *label, gfx::Renderable *hitbox = nullptr);
		TextButton(const TextButton &copy);
		~TextButton();

		void setGraphic(gfx::Button *graphic);
		gfx::Button *getGraphic();

		void setHitbox(gfx::Renderable *hitbox);
		gfx::Renderable *getHitbox();

		void setLabel(const char *label);
		void setLabel(const std::string &label);
		std::string getLabel() const;

		gfx::TextField *getLabelField() const;

		virtual void setWidth(float width);
		virtual float getWidth();

		virtual void setHeight(float height);
		virtual float getHeight();

		virtual void setEnabled(bool enabled);

		virtual void onEvent(Event *e);
		
	protected:

		base::Handle<gfx::Button> mGraphic;
		base::Handle<gfx::TextField> mLabel;

		void init();

		void updateLabelPos();
	};

}
}
