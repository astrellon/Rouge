#pragma once

#include "ievent_listener.h"
using namespace am::ui;

#include "icontroller.h"

namespace am {
namespace base {

	class Character;

	class PlayerController : public IController, public IEventListener {
	public:
		PlayerController();
		~PlayerController();

		virtual void onEvent(KeyboardEvent *e);
		virtual void update(Character *character, float dt);

	protected:

		float mMoveX;
		float mMoveY;
		bool mRunning;
	};

}
}
