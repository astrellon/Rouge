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
	class OptionsPanel;
}
using namespace am::ui;

namespace sys {

	class RougeSystem : public GameSystem {
	public:

		~RougeSystem();

		virtual void init();
		virtual void reshape(int width, int height);

		virtual void onEvent(am::ui::Event *e);

		static RougeSystem *createRougeSystem(ISystem *linked, Engine *engine, GfxEngine *gfxEngine,
			MouseManager *mouseManager);

		static RougeSystem *getRougeSystem();

		void newGame();

	protected:
		
		RougeSystem(ISystem *linked, Engine *engine, GfxEngine *gfxEngine,
			MouseManager *mouseManager);

		Handle<MainMenu> mMainMenu;
		Handle<OptionsPanel> mOptionsPanel;

		static RougeSystem *sRougeSystem;
	};

}
}
