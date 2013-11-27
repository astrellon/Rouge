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

		base::Handle<TextButton> mStartGame;
		base::Handle<TextButton> mLoadGame;
		base::Handle<TextButton> mEditor;
		base::Handle<TextButton> mOptions;
		base::Handle<TextButton> mQuit;

		RougeSystem *mRougeSystem;

		void initButton(TextButton *btn);
	};

}
}
