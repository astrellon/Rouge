#pragma once

#include "game_system.h"

#include <base/handle.h>
using namespace am::base;

namespace am {

namespace game {
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
	class Image;
	class UIComponent;
}
using namespace am::ui;

namespace sys {

	class OsSystem;

	class UnitTestSystem : public GameSystem {
	public:

		~UnitTestSystem();

		virtual void init();
		virtual void reshape(int width, int height);

		virtual void onKeyUp(int key);

		static UnitTestSystem *createUnitTestSystem(OsSystem *linked, Engine *engine);
		static UnitTestSystem *getUnitTestSystem();

	protected:
		
		UnitTestSystem(OsSystem *linked, Engine *engine);
		
		static UnitTestSystem *sUnitTestSystem;
	};

}
}
