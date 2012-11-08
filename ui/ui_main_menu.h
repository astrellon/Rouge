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
	class TextButton;

	class MainMenu : public UIComponent, public IEventListener {
	public:
		MainMenu(RougeSystem *system);
		~MainMenu();

		virtual void onEvent(Event *e);

	protected:

		Handle<TextButton> mStartGame;
		Handle<TextButton> mLoadGame;
		Handle<TextButton> mEditor;
		Handle<TextButton> mOptions;
		Handle<TextButton> mQuit;

		RougeSystem *mRougeSystem;

		void initButton(TextButton *btn);
	};

}
}
