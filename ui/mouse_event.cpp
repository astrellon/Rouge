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
		mLocalMouseY(y),
		mOriginalTarget(nullptr)
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
		mLocalMouseY(localY),
		mOriginalTarget(target)
	{
		setType(MouseEventTypeNames[mouseType]);
	}
	MouseEvent::MouseEvent(const MouseEvent &copy) :
		Event(copy.mType.c_str()),
		mMouseEventType(copy.mMouseEventType),
		mMouseButton(copy.mMouseButton),
		mMouseX(copy.mMouseX),
		mMouseY(copy.mMouseY),
		mLocalMouseX(copy.mLocalMouseX),
		mLocalMouseY(copy.mLocalMouseY),
		mOriginalTarget(copy.mOriginalTarget)
	{

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
		return dynamic_cast<Renderable *>(mEventTarget.get());
	}
	Renderable *MouseEvent::getOriginalTarget() const
	{
		return mOriginalTarget;
	}

}
}
