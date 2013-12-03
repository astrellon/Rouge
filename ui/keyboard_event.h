#pragma once

#include "event.h"
#include "keyboard_common.h"

namespace am {
namespace ui {

	class KeyboardEvent : public Event
	{
	public:
		KeyboardEvent(Keyboard::EventType type, int key, bool systemKey);
		~KeyboardEvent();

		int getKey() const;
		bool isSystemKey() const;

		Keyboard::EventType getKeyboardEventType() const;

	protected:

		Keyboard::EventType mKeyboardEventType;
		int mKey;
		bool mSystemKey;
	};

}
}
