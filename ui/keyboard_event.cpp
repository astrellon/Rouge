#include "keyboard_event.h"

#include "keyboard_manager.h"

namespace am {
namespace ui {

	KeyboardEvent::KeyboardEvent(KeyboardEventType type, int key, bool systemKey) :
		Event(),
		mKeyboardEventType(type),
		mKey(key),
		mSystemKey(systemKey)
	{
		setType(KeyboardEventTypeNames[type]);
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

	KeyboardEventType KeyboardEvent::getKeyboardEventType() const
	{
		return mKeyboardEventType;
	}

}
}
