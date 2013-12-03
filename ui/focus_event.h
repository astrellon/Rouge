#pragma once

#include <base/handle.h>

#include <gfx/gfx_renderable.h>

#include "event.h"

namespace am {
namespace ui {

	enum FocusEventType 
	{
		FOCUS, BLUR
	};

	class FocusEvent : public Event 
	{
	public:
		FocusEvent(FocusEventType type, gfx::Renderable *target);
		~FocusEvent();

		FocusEventType getFocusEventType() const;
		gfx::Renderable *getTarget() const;

	protected:

		FocusEventType mFocusEventType;
		base::Handle<gfx::Renderable> mTarget;
	};

}
}
