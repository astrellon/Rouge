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

namespace ui {
	class MouseManager;
	class Image;
	class UIComponent;
}

namespace sys {

	class MemoryTestSystem : public GameSystem {
	public:

		~MemoryTestSystem();

		virtual void init();
		
		static MemoryTestSystem *createMemoryTestSystem(OsSystem *linked, game::Engine *engine);
		static MemoryTestSystem *getMemoryTestSystem();

	protected:
		
		MemoryTestSystem(OsSystem *linked, Engine *engine);
		
		static MemoryTestSystem *sMemorySystem;
	};

}
}
