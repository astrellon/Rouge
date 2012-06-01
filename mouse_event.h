#pragma once

#include "event.h"

#include "mouse_common.h"

namespace am {
namespace gfx {
	class Renderable;
}

using namespace am::gfx;

namespace ui {

	class MouseEvent : public Event {
	public:
		MouseEvent(const char *type, MouseEventType mouseType, MouseButton mouseButton, int x, int y);
		MouseEvent(const char *type, MouseEventType mouseType, MouseButton mouseButton, int x, int y, Renderable *target, int localX, int localY);
		~MouseEvent();

		virtual MouseButton getMouseButton() const;
		virtual MouseEventType getMouseEventType() const;
		virtual int getMouseX() const;
		virtual int getMouseY() const;

		virtual int getLocalMouseX() const;
		virtual int getLocalMouseY() const;

		virtual Renderable *getTarget() const;

	protected:

		MouseButton mMouseButton;
		int mMouseX;
		int mMouseY;

		int mLocalMouseX;
		int mLocalMouseY;

		MouseEventType mMouseEventType;

		Renderable *mTarget;
	};

}
}
