#pragma once

#include <base/imanaged.h>
using namespace am::base;

namespace am {
namespace game {

	class Character;

	class IController : public IManaged {
	public:

		IController();

		virtual void update(Character *character, float dt) = 0;
		virtual void detach() = 0;

		virtual void setActive(bool active);
		virtual bool isActive() const;

	protected:
		bool mActive;

	};

}
}
