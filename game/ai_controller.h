#pragma once

#include "icontroller.h"

namespace am {
namespace game {

	class Character;

	class AiController : public IController {
	public:
		AiController();
		~AiController();

		virtual void update(Character *character, float dt);
		virtual void detach();

		//void performAction();

	protected:

		//int mMoveX;
		//int mMoveY;
		//bool mRunning;
		bool mRemoved;
		bool mAttached;
		Character *mCharacter;
	};

}
}
