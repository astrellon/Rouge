#pragma once

#include "keyboard_common.h"
#include "event_interface.h"

namespace am {
namespace ui {

	class KeyboardManager : public EventInterface {
	public:
		KeyboardManager();
		~KeyboardManager();

		virtual void onKeyDown(Key key);
		virtual void onKeyUp(Key key);
		virtual void onKeyPress(char key);

		virtual const bool *getKeysDown() const;
		virtual bool isKeyDown(int key) const;

		virtual const bool *getKeysPressed() const;
		virtual bool isKeyPressed(int key) const;

		virtual void onNewGameTick();

		static void setManager(KeyboardManager *manager);
		static KeyboardManager *getManager();

	protected:

		bool mKeys[256];
		bool mKeysPressed[256];

		static KeyboardManager *sMainManager;
	};

}
}
