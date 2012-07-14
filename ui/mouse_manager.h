#pragma once

#include <map>
using namespace std;

#include <base/handle.h>
using namespace am::base;

#include "mouse_common.h"
#include "mouse_event.h"
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

		Renderable *checkForMouseEvent(Renderable *target, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY);
		Renderable *getUnderMouse() const;
		bool getButtonDown(MouseButton button);

		void onMouseDown(MouseButton mouseButton, int x, int y);
		void onMouseMove(MouseButton mouseButton, int x, int y);
		void onMouseUp(MouseButton mouseButton, int x, int y);

		void setDragOffset(int x, int y);
		int getDragOffsetX() const;
		int getDragOffsetY() const;

		void clearCurrentlyFiring();

		static void setManager(MouseManager *manager);
		static MouseManager *getManager();

	protected:

		Renderable *mUnderMouse;

		bool mStopCurrentEvents;
		bool mFiredEvent;
		Handle<MouseEvent> mCurrentEvent;
		typedef map<MouseButton, bool> MouseButtonMap;
		MouseButtonMap mMouseButtonsDown;

		int mDragOffsetX;
		int mDragOffsetY;

		static MouseManager *sMainManager;

		void fireMouseEvent(Renderable *target, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY);
	};

}
}
