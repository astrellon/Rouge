#pragma once

#include "event.h"

namespace am {
namespace ui {

	class IEventListener {
	public:
		virtual void onEvent(Event &e) = 0;
	};

}
}
