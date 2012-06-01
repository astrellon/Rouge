#pragma once

#include "mouse_common.h"
#include <map>

using namespace std;

namespace am {
namespace gfx {
	class Renderable;
	class GfxEngine;
}

using namespace am::gfx;

namespace ui {

	class MouseManager {
	public:

		MouseManager(GfxEngine *engine);
		~MouseManager();

		void setGfxEngine(GfxEngine *engine);
		GfxEngine *getGfxEngine() const;

		bool checkForMouseEvent(Renderable *target, const char *type, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY);
		Renderable *getUnderMouse() const;
		bool getButtonDown(MouseButton button);

		void onMouseDown(MouseButton mouseButton, int x, int y);
		void onMouseMove(MouseButton mouseButton, int x, int y);
		void onMouseUp(MouseButton mouseButton, int x, int y);

	protected:

		Renderable *mUnderMouse;
		map<MouseButton, bool> mMouseButtonsDown;
		GfxEngine *mGfxEngine;

		void fireMouseEvent(Renderable *target, const char *type, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY);
	};

}
}
