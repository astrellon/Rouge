#pragma once

#include <base/handle.h>

#include "ui_component.h"
#include "ievent_listener.h"

namespace am {

namespace sys {
	class RougeSystem;
}

namespace ui {
	class TextButton;

	class IngameMenu : public UIComponent, public IEventListener 
	{
	public:
		IngameMenu(sys::RougeSystem *system);
		~IngameMenu();

		virtual void onEvent(Event *e);

	protected:

		base::Handle<TextButton> mResumeGame;
		base::Handle<TextButton> mSaveGame;
		base::Handle<TextButton> mLoadGame;
		base::Handle<TextButton> mOptions;
		base::Handle<TextButton> mToMainMenu;
		base::Handle<TextButton> mQuit;

		sys::RougeSystem *mRougeSystem;

		void initButton(TextButton *btn);
	};

}
}
