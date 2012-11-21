#pragma once

#include <base/imanaged.h>

namespace am {
namespace game {

	class Component : public am::base::IManaged {
	public:

		typedef int ComponentKey;

		Component();
		~Component();

		virtual const char *getCompName() const = 0;
		virtual ComponentKey getCompKey() const = 0;
		virtual void update(float dt) {}

	};

}
}
