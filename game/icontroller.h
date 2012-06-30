#pragma once

#include <base/imanaged.h>
using namespace am::base;

namespace am {
namespace game {

	class Character;

	class IController : public IManaged {
	public:

		virtual void update(Character *character, float dt) = 0;

	};

}
}
