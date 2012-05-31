#include "keyboard_event.h"

namespace am {
namespace ui {

	KeyboardEvent::KeyboardEvent(const char *type, const bool *keys, int key) :
		Event(type),
		mKeys(keys),
		mKey(key)
	{
	}
	KeyboardEvent::~KeyboardEvent()
	{
	}

	const bool *KeyboardEvent::getKeys() const
	{
		return mKeys;
	}
	int KeyboardEvent::getKey() const
	{
		return mKey;
	}

}
}
