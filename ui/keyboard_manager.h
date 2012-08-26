#pragma once

#include "keyboard_common.h"
#include "event_manager.h"

namespace am {
namespace ui {

	class KeyboardManager : public EventManager {
	public:
		KeyboardManager();
		~KeyboardManager();

		virtual void onKeyDown(int key, bool systemKey);
		virtual void onKeyUp(int key);

		virtual const bool *getKeysDown() const;
		virtual bool isKeyDown(int key) const;

		static void setManager(KeyboardManager *manager);
		static KeyboardManager *getManager();

	protected:

		bool *mKeys;

		static KeyboardManager *sMainManager;
	};

}
}
