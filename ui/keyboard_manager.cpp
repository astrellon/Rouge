#include "keyboard_manager.h"

#include <base/handle.h>

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

	void KeyboardManager::onKeyDown(ui::Keyboard::Key key)
	{
		mKeys[key] = true;
		mKeysPressed[key] = true;
		base::Handle<KeyboardEvent> e(new KeyboardEvent(ui::Keyboard::KEY_DOWN, key, true));
		fireEvent<KeyboardEvent>(e);
	}
	void KeyboardManager::onKeyUp(ui::Keyboard::Key key)
	{
		mKeys[key] = false;
		base::Handle<KeyboardEvent> e(new KeyboardEvent(ui::Keyboard::KEY_UP, key, true));
		fireEvent<KeyboardEvent>(e);
	}
	void KeyboardManager::onKeyPress(char key)
	{
		base::Handle<KeyboardEvent> e(new KeyboardEvent(ui::Keyboard::KEY_PRESS, key, false));
		fireEvent<KeyboardEvent>(e);
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
