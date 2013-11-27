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

		base::Handle<Image> mBackground;
		base::Handle<Checkbox> mFullscreen;
		base::Handle<TextButton> mClose;
		base::Handle<TextButton> mApply;

		RougeSystem *mRougeSystem;
	};

}
}
