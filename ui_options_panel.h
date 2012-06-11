#pragma once

#include "ui_component.h"

#include "handle.h"

#include "ievent_listener.h"

using namespace am::util;

namespace am {
namespace gfx {
	class GfxEngine;
}

using namespace am::gfx;

namespace ui {

	class Button;
	class Checkbox;
	class Image;

	class OptionsPanel : public UIComponent, public IEventListener {
	public:
		OptionsPanel(GfxEngine *engine);
		~OptionsPanel();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void onEvent(Event *e);
		virtual void onEvent(DataEvent *e);

	protected:

		Handle<Image> mBackground;
		Handle<Checkbox> mFullscreen;
		Handle<Button> mClose;
		Handle<Button> mApply;
	};

}
}
