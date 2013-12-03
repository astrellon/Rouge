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

	class MainMenu : public UIComponent, public IEventListener 
	{
	public:
		MainMenu(sys::RougeSystem *system);
		~MainMenu();

		virtual void onEvent(Event *e);

	protected:

		base::Handle<TextButton> mStartGame;
		base::Handle<TextButton> mLoadGame;
		base::Handle<TextButton> mEditor;
		base::Handle<TextButton> mOptions;
		base::Handle<TextButton> mQuit;

		sys::RougeSystem *mRougeSystem;

		void initButton(TextButton *btn);
	};

}
}
