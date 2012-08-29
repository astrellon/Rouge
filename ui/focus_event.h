#pragma once

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_renderable.h>
using namespace am::gfx;

#include "event.h"

namespace am {
namespace ui {

	enum FocusEventType {
		FOCUS, BLUR
	};

	class FocusEvent : public Event {
	public:
		FocusEvent(FocusEventType type, Renderable *target);
		~FocusEvent();

		FocusEventType getFocusEventType() const;
		Renderable *getTarget() const;

	protected:

		FocusEventType mFocusEventType;
		Handle<Renderable> mTarget;
	};

}
}
