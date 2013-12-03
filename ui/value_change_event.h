#pragma once

#include "event.h"

namespace am {
namespace ui {

	class UIComponent;

	class ValueChangeEvent : public Event
	{
	public:
		ValueChangeEvent(UIComponent *target, int value);
		~ValueChangeEvent();

		UIComponent *getTarget() const;
		int getData() const;

	protected:

		UIComponent *mTarget;
		int mData;

	};

}
}
