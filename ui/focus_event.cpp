#include "focus_event.h"

namespace am {
namespace ui {

	FocusEvent::FocusEvent(FocusEventType type, Renderable *target) :
		Event(type == FOCUS ? "focus" : "blur"),
		mFocusEventType(type),
		mTarget(target)
	{

	}
	FocusEvent::~FocusEvent()
	{

	}

	FocusEventType FocusEvent::getFocusEventType() const
	{
		return mFocusEventType;
	}
	Renderable *FocusEvent::getTarget() const
	{
		return mTarget;
	}

}
}
