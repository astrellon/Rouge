#pragma once

#include "event.h"

#include "mouse_common.h"

namespace am {
namespace gfx {
	class Renderable;
}

using namespace am::gfx;

namespace ui {

	class MouseManager;

	class MouseEvent : public Event {
	public:
		MouseEvent(MouseManager *manager, MouseEventType mouseType, MouseButton mouseButton, int x, int y);
		MouseEvent(MouseManager *manager, MouseEventType mouseType, MouseButton mouseButton, int x, int y, Renderable *target, int localX, int localY);
		~MouseEvent();

		virtual MouseButton getMouseButton() const;
		virtual MouseEventType getMouseEventType() const;
		virtual int getMouseX() const;
		virtual int getMouseY() const;

		virtual int getLocalMouseX() const;
		virtual int getLocalMouseY() const;

		virtual Renderable *getTarget() const;

		virtual MouseManager *getManager();

	protected:

		MouseButton mMouseButton;
		MouseManager *mManager;
		int mMouseX;
		int mMouseY;

		int mLocalMouseX;
		int mLocalMouseY;

		MouseEventType mMouseEventType;

		Renderable *mTarget;
	};

}
}
