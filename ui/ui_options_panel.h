#pragma once

#include <base/handle.h>
using namespace am::base;

#include "ui_component.h"
#include "ievent_listener.h"

namespace am {

namespace sys {
	class RougeSystem;
}
using namespace am::sys;

namespace ui {

	class Button;
	class Checkbox;
	class Image;

	class OptionsPanel : public UIComponent, public IEventListener {
	public:
		OptionsPanel(RougeSystem *system);
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

		RougeSystem *mRougeSystem;
	};

}
}
