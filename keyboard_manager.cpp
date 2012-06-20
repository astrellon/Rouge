#include "keyboard_manager.h"

#include "handle.h"
#include "keyboard_event.h"

namespace am {
namespace ui {

	KeyboardManager *KeyboardManager::sMainManager = NULL;

	KeyboardManager::KeyboardManager() :
		mKeys(new bool[256])
	{
		for (int i = 0; i < 256; i++)
		{
			mKeys[i] = false;
		}
	}
	KeyboardManager::~KeyboardManager()
	{
		delete mKeys;
	}

	void KeyboardManager::onKeyDown(int key)
	{
		mKeys[key] = true;
		Handle<KeyboardEvent> e(new KeyboardEvent(KEY_DOWN, key));
		fireEvent<KeyboardEvent>(e.get());
	}
	void KeyboardManager::onKeyUp(int key)
	{
		mKeys[key] = false;
		Handle<KeyboardEvent> e(new KeyboardEvent(KEY_UP, key));
		fireEvent<KeyboardEvent>(e.get());
	}

	const bool *KeyboardManager::getKeysDown() const
	{
		return mKeys;
	}
	bool KeyboardManager::isKeyDown(int key) const
	{
		return mKeys[key];
	}

	void KeyboardManager::setManager(KeyboardManager *manager)
	{
		sMainManager = manager;
	}
	KeyboardManager *KeyboardManager::getManager()
	{
		return sMainManager;
	}

}
}
