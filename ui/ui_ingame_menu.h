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
	class TextButton;

	class IngameMenu : public UIComponent, public IEventListener {
	public:
		IngameMenu(RougeSystem *system);
		~IngameMenu();

		virtual void onEvent(Event *e);

	protected:

		Handle<TextButton> mResumeGame;
		Handle<TextButton> mSaveGame;
		Handle<TextButton> mLoadGame;
		Handle<TextButton> mOptions;
		Handle<TextButton> mToMainMenu;
		Handle<TextButton> mQuit;

		RougeSystem *mRougeSystem;

		void initButton(TextButton *btn);
	};

}
}
