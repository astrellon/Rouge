#pragma once

#include <base/imanaged.h>

#include <string>

#include <math/vector.h>

#include "iaction.h"

namespace am {
namespace game {

	class Character;

	class MoveAction : public IAction 
	{
	public:
		
		MoveAction(float destX, float destY);
		MoveAction(math::Vector2f dest);

		void setDestination(float destX, float destY);
		void setDestination(const math::Vector2f &dest);
		math::Vector2f getDestination() const;

		const char *getName() const;

		bool update(Character *obj, float dt);

	protected:

		math::Vector2f mDestination;

	};

}
}
