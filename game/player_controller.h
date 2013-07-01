#pragma once

#include <ui/ievent_listener.h>
using namespace am::ui;

#include <base/handle.h>
using namespace am::base;


#include "character.h"
#include "iaction.h"
#include "icontroller.h"

namespace am {
namespace game {

	class PlayerController : public IController, public IEventListener {
	public:
		PlayerController();
		~PlayerController();

		virtual void onEvent(KeyboardEvent *e);
		virtual void update(Character *character, float dt);
		virtual void detach();

		//void performAction();

	protected:

		bool mRunning;
		bool mRemoved;
		bool mAttached;
		Handle<Character> mCharacter;
		Handle<IAction> mAction;

		void move(int dx, int dy);
		void wait(float dt);

		void setGameTick(float dt);
	};

}
}
