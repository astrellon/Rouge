#pragma once

#include <base/handle.h>
using namespace am::base;

#include "ui_component.h"
#include "ievent_listener.h"

using namespace am::util;

namespace am {

namespace sys {
	class RougeSystem;
}
using namespace am::sys;

namespace ui {
	class Button;

	class MainMenu : public UIComponent, public IEventListener {
	public:
		MainMenu(RougeSystem *system);
		~MainMenu();

		virtual void onEvent(Event *e);

	protected:

		Handle<Button> mStartGame;
		Handle<Button> mLoadGame;
		Handle<Button> mEditor;
		Handle<Button> mOptions;
		Handle<Button> mQuit;

		RougeSystem *mRougeSystem;

		void initButton(Button *btn);
	};

}
}
