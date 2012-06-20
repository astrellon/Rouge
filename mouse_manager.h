#pragma once

#include <map>
using namespace std;

#include "mouse_common.h"
#include "event_manager.h"

namespace am {
namespace gfx {
	class Renderable;
}

using namespace am::gfx;

namespace ui {

	class MouseManager : public EventManager {
	public:

		MouseManager();
		~MouseManager();

		bool checkForMouseEvent(Renderable *target, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY);
		Renderable *getUnderMouse() const;
		bool getButtonDown(MouseButton button);

		void onMouseDown(MouseButton mouseButton, int x, int y);
		void onMouseMove(MouseButton mouseButton, int x, int y);
		void onMouseUp(MouseButton mouseButton, int x, int y);

		static void setManager(MouseManager *manager);
		static MouseManager *getManager();

	protected:

		Renderable *mUnderMouse;
		typedef map<MouseButton, bool> MouseButtonMap;
		MouseButtonMap mMouseButtonsDown;

		static MouseManager *sMainManager;

		void fireMouseEvent(Renderable *target, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY);
	};

}
}
