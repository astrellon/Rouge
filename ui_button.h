#pragma once

#include "ievent_listener.h"
#include "event_manager.h"

namespace am {
namespace gfx {
	class Sprite;
}

using namespace am::gfx;

namespace ui {

	class Button : public IEventListener, public EventManager {
	public:
		Button(Sprite *graphic);
		~Button();

	protected:

		Sprite *mGraphic;
	};

}
}
