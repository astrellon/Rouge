#pragma once

#include "ui_component.h"
#include "handle.h"
#include "ievent_listener.h"

using namespace am::util;

namespace am {

namespace sys {
	class RougeSystem;
}
using namespace am::sys;

namespace ui {
	class Button;

	class IngameMenu : public UIComponent, public IEventListener {
	public:
		IngameMenu(RougeSystem *system);
		~IngameMenu();

		virtual void onEvent(Event *e);

	protected:

		Handle<Button> mResumeGame;
		Handle<Button> mSaveGame;
		Handle<Button> mLoadGame;
		Handle<Button> mOptions;
		Handle<Button> mToMainMenu;
		Handle<Button> mQuit;

		RougeSystem *mRougeSystem;

		void initButton(Button *btn);
	};

}
}
