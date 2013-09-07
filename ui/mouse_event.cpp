#include "mouse_event.h"

#include <gfx/gfx_renderable.h>
using namespace am::gfx;

namespace am {
namespace ui {

	MouseEvent::MouseEvent(MouseEventType mouseType, MouseButton mouseButton, int x, int y) :
		Event(),
		mMouseEventType(mouseType),
		mMouseButton(mouseButton),
		mMouseX(x),
		mMouseY(y),
		mLocalMouseX(x),
		mLocalMouseY(y)
	{
		setType(MouseEventTypeNames[mouseType]);
	}
	MouseEvent::MouseEvent(MouseEventType mouseType, MouseButton mouseButton, int x, int y,
		Renderable *target, int localX, int localY) :
		Event(),
		mMouseEventType(mouseType),
		mMouseButton(mouseButton),
		mMouseX(x),
		mMouseY(y),
		mLocalMouseX(localX),
		mLocalMouseY(localY)
	{
		setType(MouseEventTypeNames[mouseType]);
		mEventTarget = target;
	}
	MouseEvent::~MouseEvent()
	{
	}

	MouseButton MouseEvent::getMouseButton() const
	{
		return mMouseButton;
	}
	MouseEventType MouseEvent::getMouseEventType() const
	{
		return mMouseEventType;
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
		return reinterpret_cast<Renderable *>(mEventTarget.get());
	}

}
}
