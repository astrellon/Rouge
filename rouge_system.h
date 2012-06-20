#pragma once

#include "game_system.h"

#include "handle.h"
using namespace am::util;

namespace am {

namespace base {
	class Engine;
}
using namespace am::base;

namespace gfx {
	class GfxEngine;
	class Layer;
}
using namespace am::gfx;

namespace ui {
	class MouseManager;
	class MainMenu;
	class IngameMenu;
	class OptionsPanel;
}
using namespace am::ui;

namespace sys {

	class RougeSystem : public GameSystem {
	public:

		~RougeSystem();

		virtual void init();
		virtual void reshape(int width, int height);

		virtual void onKeyUp(int key);

		static RougeSystem *createRougeSystem(ISystem *linked, Engine *engine);
		static RougeSystem *getRougeSystem();

		void newGame();
		void resumeGame();
		void quitGame();
		void toMainMenu();
		void showOptionsPanel();
		void closeOptionsPanel();

	protected:
		
		RougeSystem(ISystem *linked, Engine *engine);

		Handle<MainMenu> mMainMenu;
		Handle<OptionsPanel> mOptionsPanel;
		Handle<IngameMenu> mIngameMenu;

		static RougeSystem *sRougeSystem;
	};

}
}
