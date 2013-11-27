#pragma once

#include <base/imanaged.h>

#include <string>
using namespace std;

#include <math/vector.h>
using namespace am::math;

#include "iaction.h"

namespace am {
namespace game {

	class Character;

	class MoveAction : public IAction {
	public:
		
		MoveAction(float destX, float destY);
		MoveAction(Vector2f dest);

		void setDestination(float destX, float destY);
		void setDestination(const Vector2f &dest);
		Vector2f getDestination() const;

		const char *getName() const;

		bool update(Character *obj, float dt);

	protected:

		Vector2f mDestination;

	};

}
}
