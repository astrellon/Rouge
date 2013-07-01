#include "keyboard_manager.h"

#include <base/handle.h>
using namespace am::base;

#include "keyboard_event.h"

namespace am {
namespace ui {

	KeyboardManager *KeyboardManager::sMainManager;

	KeyboardManager::KeyboardManager()
	{
		for (int i = 0; i < 256; i++)
		{
			mKeys[i] = false;
			mKeysPressed[i] = false;
		}
	}
	KeyboardManager::~KeyboardManager()
	{
	}

	void KeyboardManager::onKeyDown(int key, bool systemKey)
	{
		if (systemKey)
		{
			mKeys[key] = true;
			mKeysPressed[key] = true;
		}
		Handle<KeyboardEvent> e(new KeyboardEvent(KEY_DOWN, key, systemKey));
		fireEvent<KeyboardEvent>(e.get());
	}
	void KeyboardManager::onKeyUp(int key)
	{
		mKeys[key] = false;
		Handle<KeyboardEvent> e(new KeyboardEvent(KEY_UP, key, true));
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

	const bool *KeyboardManager::getKeysPressed() const
	{
		return mKeysPressed;
	}
	bool KeyboardManager::isKeyPressed(int key) const
	{
		return mKeysPressed[key];
	}

	void KeyboardManager::onNewGameTick()
	{
		for (int i = 0; i < 256; i++)
		{
			mKeysPressed[i] = false;
		}
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
