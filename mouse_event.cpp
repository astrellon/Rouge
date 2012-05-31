#include "mouse_event.h"

#include "gfx/gfx_renderable.h"

using namespace am::gfx;

namespace am {
namespace ui {

	MouseEvent::MouseEvent(const char *type, int mouseButton, int x, int y) :
		Event(type),
		mMouseButton(mouseButton),
		mMouseX(x),
		mMouseY(y),
		mLocalMouseX(x),
		mLocalMouseY(y),
		mTarget(NULL)
	{
	}
	MouseEvent::MouseEvent(const char *type, int mouseButton, int x, int y,
		Renderable *target, int localX, int localY) :
		Event(type),
		mMouseButton(mouseButton),
		mMouseX(x),
		mMouseY(y),
		mLocalMouseX(localX),
		mLocalMouseY(localY),
		mTarget(target)
	{
	}
	MouseEvent::~MouseEvent()
	{
	}

	int MouseEvent::getMouseButton() const
	{
		return mMouseButton;
	}
	int MouseEvent::getMouseX() const
	{
		return mMouseX;
	}
	int MouseEvent::getMouseY() const
	{
		return mMouseY;
	}

	int MouseEvent::getLocalMouseX() const
	{
		return mLocalMouseX;
	}
	int MouseEvent::getLocalMouseY() const
	{
		return mLocalMouseY;
	}

	Renderable *MouseEvent::getTarget() const
	{
		return mTarget;
	}

}
}
