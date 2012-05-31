#pragma once

#include "event.h"

namespace am {
namespace ui {

	class KeyboardEvent : public Event {
	public:
		KeyboardEvent(const char *type, const bool *keys, int key);
		~KeyboardEvent();

		const bool *getKeys() const;
		int getKey() const;

	protected:

		const bool *mKeys;
		int mKey;
	};

}
}
