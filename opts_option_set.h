#pragma once

#include "event_manager.h"

namespace am {
namespace opts {

	class OptionSet : public am::ui::EventManager {
	public:
		OptionSet();
		~OptionSet();
	};

}
}
