#include "value_change_event.h"

#include "ui_component.h"

namespace am {
namespace ui {

	ValueChangeEvent::ValueChangeEvent(UIComponent *target, int data) :
		Event("change", target),
		mTarget(target),
		mData(data)
	{
		if (target)
		{
			target->retain();
		}
	}
	ValueChangeEvent::~ValueChangeEvent()
	{
		if (mTarget)
		{
			mTarget->release();
		}
	}

	UIComponent *ValueChangeEvent::getTarget() const
	{
		return mTarget;
	}
	int ValueChangeEvent::getData() const
	{
		return mData;
	}
}
}
