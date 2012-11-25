#pragma once

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_renderable.h>
using namespace am::gfx;

#include "event_interface.h"
#include "event.h"

namespace am {
namespace ui {

	class FocusManager : public EventInterface {
	public:
		FocusManager();
		~FocusManager();

		virtual void setFocus(Renderable *renderable);
		virtual Renderable *getFocus() const;

		static void setManager(FocusManager *manager);
		static FocusManager *getManager();

	protected:

		Handle<Renderable> mCurrentFocus;

		static FocusManager *sManager;

	};

}
}
