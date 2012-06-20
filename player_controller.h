#pragma once

#include "icontroller.h"

namespace am {
namespace base {

	class Character;

	class PlayeController : public IController {
	public:
		PlayerController();
		~PlayerController();

		virtual void update(Character *character, float dt);
	};

}
}
