#include "mouse_event.h"

#include <gfx/gfx_renderable.h>

namespace am {
namespace ui {

	MouseEvent::MouseEvent(Mouse::EventType mouseType, Mouse::Button mouseButton, int x, int y) :
		Event(),
		mMouseEventType(mouseType),
		mMouseButton(mouseButton),
		mMouseX(x),
		mMouseY(y),
		mLocalMouseX(x),
		mLocalMouseY(y),
		mOriginalTarget(nullptr)
	{
		setType(Mouse::EventTypeNames[mouseType]);
	}
	MouseEvent::MouseEvent(Mouse::EventType mouseType, Mouse::Button mouseButton, int x, int y,
			gfx::Renderable *target, int localX, int localY) :
		Event(),
		mMouseEventType(mouseType),
		mMouseButton(mouseButton),
		mMouseX(x),
		mMouseY(y),
		mLocalMouseX(localX),
		mLocalMouseY(localY),
		mOriginalTarget(target)
	{
		setType(Mouse::EventTypeNames[mouseType]);
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

	Mouse::Button MouseEvent::getMouseButton() const
	{
		return mMouseButton;
	}
	Mouse::EventType MouseEvent::getMouseEventType() const
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

	gfx::Renderable *MouseEvent::getTarget() const
	{
		return dynamic_cast<gfx::Renderable *>(mEventTarget.get());
	}

	void MouseEvent::setOriginalTarget(gfx::Renderable *target)
	{
		mOriginalTarget = target;
	}
	gfx::Renderable *MouseEvent::getOriginalTarget() const
	{
		return mOriginalTarget;
	}

}
}
