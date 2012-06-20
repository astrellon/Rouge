#pragma once

#include "imanaged.h"
using namespace am::util;

namespace am {
namespace base {

	class Character;

	class IController : public IManaged {
	public:

		virtual void update(Character *character, float dt) = 0;

	};

}
}
