#pragma once

#include "event.h"

namespace am {
namespace gfx {
	class Renderable;
}

using namespace am::gfx;

namespace ui {

	class MouseEvent : public Event {
	public:
		MouseEvent(const char *type, int mouseButton, int x, int y);
		MouseEvent(const char *type, int mouseButton, int x, int y, Renderable *target, int localX, int localY);
		~MouseEvent();

		virtual int getMouseButton() const;
		virtual int getMouseX() const;
		virtual int getMouseY() const;

		virtual int getLocalMouseX() const;
		virtual int getLocalMouseY() const;

		virtual Renderable *getTarget() const;

	protected:

		int mMouseButton;
		int mMouseX;
		int mMouseY;

		int mLocalMouseX;
		int mLocalMouseY;

		Renderable *mTarget;
	};

}
}
