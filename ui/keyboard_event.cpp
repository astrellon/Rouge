#include "keyboard_event.h"

#include "keyboard_manager.h"

namespace am {
namespace ui {

	KeyboardEvent::KeyboardEvent(Keyboard::EventType type, int key, bool systemKey) :
		Event(),
		mKeyboardEventType(type),
		mKey(key),
		mSystemKey(systemKey)
	{
		setType(Keyboard::EventTypeNames[type]);
	}
	KeyboardEvent::~KeyboardEvent()
	{
	}

	int KeyboardEvent::getKey() const
	{
		return mKey;
	}
	bool KeyboardEvent::isSystemKey() const
	{
		return mSystemKey;
	}

	Keyboard::EventType KeyboardEvent::getKeyboardEventType() const
	{
		return mKeyboardEventType;
	}

}
}
