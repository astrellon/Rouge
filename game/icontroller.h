#pragma once

#include <base/imanaged.h>

namespace am {
namespace game {

	class Character;

	class IController : public base::IManaged 
	{
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
