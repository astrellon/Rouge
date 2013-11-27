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
		MouseEvent(Mouse::EventType mouseType, Mouse::Button mouseButton, int x, int y);
		MouseEvent(Mouse::EventType mouseType, Mouse::Button mouseButton, int x, int y, Renderable *target, int localX, int localY);
		MouseEvent(const MouseEvent &copy);
		~MouseEvent();

		virtual Mouse::Button getMouseButton() const;
		virtual Mouse::EventType getMouseEventType() const;
		virtual int getMouseX() const;
		virtual int getMouseY() const;

		virtual int getLocalMouseX() const;
		virtual int getLocalMouseY() const;

		virtual Renderable *getTarget() const;

		virtual void setOriginalTarget(Renderable *target);
		virtual Renderable *getOriginalTarget() const;

	protected:

		Mouse::Button mMouseButton;
		int mMouseX;
		int mMouseY;

		int mLocalMouseX;
		int mLocalMouseY;

		Renderable *mOriginalTarget;
		Mouse::EventType mMouseEventType;

	};

}
}
