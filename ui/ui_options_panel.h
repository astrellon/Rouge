#pragma once

#include <base/handle.h>

#include "ui_component.h"
#include "ievent_listener.h"

namespace am {

namespace sys {
	class RougeSystem;
}
using namespace am::sys;

namespace ui {

	class TextButton;
	class Checkbox;
	class Image;

	class OptionsPanel : public UIComponent, public IEventListener {
	public:
		OptionsPanel(RougeSystem *system);
		~OptionsPanel();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void onEvent(Event *e);
		virtual void onEvent(ValueChangeEvent *e);
		//virtual void onEvent(DataEvent *e);

	protected:

		am::base::Handle<Image> mBackground;
		am::base::Handle<Checkbox> mFullscreen;
		am::base::Handle<TextButton> mClose;
		am::base::Handle<TextButton> mApply;

		RougeSystem *mRougeSystem;
	};

}
}
