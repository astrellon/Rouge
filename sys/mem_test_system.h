#pragma once

#include "game_system.h"

#include <base/handle.h>

namespace am {

namespace game {
	class Engine;
}

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

	class MemoryTestSystem : public GameSystem {
	public:

		~MemoryTestSystem();

		virtual void init();
		
		static MemoryTestSystem *createMemoryTestSystem(OsSystem *linked, Engine *engine);
		static MemoryTestSystem *getMemoryTestSystem();

	protected:
		
		MemoryTestSystem(OsSystem *linked, Engine *engine);
		
		static MemoryTestSystem *sMemorySystem;
	};

}
}
