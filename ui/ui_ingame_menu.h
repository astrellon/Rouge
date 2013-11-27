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

	class IngameMenu : public UIComponent, public IEventListener {
	public:
		IngameMenu(RougeSystem *system);
		~IngameMenu();

		virtual void onEvent(Event *e);

	protected:

		am::base::Handle<TextButton> mResumeGame;
		am::base::Handle<TextButton> mSaveGame;
		am::base::Handle<TextButton> mLoadGame;
		am::base::Handle<TextButton> mOptions;
		am::base::Handle<TextButton> mToMainMenu;
		am::base::Handle<TextButton> mQuit;

		RougeSystem *mRougeSystem;

		void initButton(TextButton *btn);
	};

}
}
