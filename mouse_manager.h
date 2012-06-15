#pragma once

#include "mouse_common.h"
#include <map>

using namespace std;

namespace am {
namespace gfx {
	class Renderable;
}

using namespace am::gfx;

namespace ui {

	class MouseManager {
	public:

		MouseManager();
		~MouseManager();

		bool checkForMouseEvent(Renderable *target, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY);
		Renderable *getUnderMouse() const;
		bool getButtonDown(MouseButton button);

		void onMouseDown(MouseButton mouseButton, int x, int y);
		void onMouseMove(MouseButton mouseButton, int x, int y);
		void onMouseUp(MouseButton mouseButton, int x, int y);

	protected:

		Renderable *mUnderMouse;
		typedef map<MouseButton, bool> MouseButtonMap;
		MouseButtonMap mMouseButtonsDown;

		void fireMouseEvent(Renderable *target, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY);
	};

}
}
