#pragma once

#include <base/handle.h>

#include <gfx/gfx_renderable.h>

#include "event_interface.h"
#include "event.h"

namespace am {
namespace ui {

	class FocusManager : public EventInterface
	{
	public:
		FocusManager();
		~FocusManager();

		virtual void setFocus(gfx::Renderable *renderable);
		virtual gfx::Renderable *getFocus() const;

		static void setManager(FocusManager *manager);
		static FocusManager *getManager();

	protected:

		base::Handle<gfx::Renderable> mCurrentFocus;

		static FocusManager *sManager;

	};

}
}
