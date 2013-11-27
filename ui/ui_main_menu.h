#pragma once

#include <base/handle.h>

#include "ui_component.h"
#include "ievent_listener.h"

using namespace am::util;

namespace am {

namespace sys {
	class RougeSystem;
}
using namespace am::sys;

namespace ui {
	class TextButton;

	class MainMenu : public UIComponent, public IEventListener {
	public:
		MainMenu(RougeSystem *system);
		~MainMenu();

		virtual void onEvent(Event *e);

	protected:

		am::base::Handle<TextButton> mStartGame;
		am::base::Handle<TextButton> mLoadGame;
		am::base::Handle<TextButton> mEditor;
		am::base::Handle<TextButton> mOptions;
		am::base::Handle<TextButton> mQuit;

		RougeSystem *mRougeSystem;

		void initButton(TextButton *btn);
	};

}
}
