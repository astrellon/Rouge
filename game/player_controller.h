#pragma once

#include <ui/ievent_listener.h>

#include <base/handle.h>

#include "character.h"
#include "iaction.h"
#include "icontroller.h"

namespace am {
namespace game {

	class PlayerController : public IController
	{
	public:
		PlayerController();
		~PlayerController();

		virtual void update(Character *character, float dt);
		virtual void onGameTick(Character *character, float dt);
		virtual void detach();

	protected:

		bool mRunning;
		bool mRemoved;
		bool mAttached;
		base::Handle<Character> mCharacter;
		base::Handle<IAction> mAction;

		void move(int dx, int dy);
		void wait(float dt);

		void setGameTick(float dt);
	};

}
}
