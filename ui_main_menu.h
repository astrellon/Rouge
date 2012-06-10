#pragma once

#include "ui_component.h"
#include "handle.h"

using namespace am::util;

namespace am {
namespace gfx {
	class GfxEngine;
}

using namespace am::gfx;

namespace ui {
	class Button;

	class MainMenu : public UIComponent {
	public:
		MainMenu(GfxEngine *engine);
		~MainMenu();

	protected:

		Handle<Button> mStartGame;
		Handle<Button> mLoadGame;
		Handle<Button> mOptions;
		Handle<Button> mQuit;

		void initButton(Button *btn);
	};

}
}
