#include "keyboard_event.h"

#include "keyboard_manager.h"

namespace am {
namespace ui {

	KeyboardEvent::KeyboardEvent(KeyboardEventType type, int key) :
		Event(),
		mKeyboardEventType(type),
		mKey(key)
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

	KeyboardEventType KeyboardEvent::getKeyboardEventType() const
	{
		return mKeyboardEventType;
	}

}
}
