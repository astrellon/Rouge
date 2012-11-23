#pragma once

#include <base/imanaged.h>

namespace am {
namespace game {

	class Component : public am::base::IManaged {
	public:

		Component();
		~Component();

		virtual const char *getCompName() const = 0;
		virtual void update(float dt) {}

	};

}
}
