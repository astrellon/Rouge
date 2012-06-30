#pragma once

#include <base/handle.h>
using namespace am::base;

#include "game_object.h"

namespace am {
namespace game {

	class Camera {
	public:
		Camera();
		~Camera();

		void followObject(GameObject *object);
		GameObject *getFollowing();

		void setDestination(float x, float y);
		float getDestinationX();
		float getDestinationY();

		float getLocationX();
		float getLocationY();

		void update(float dt);

	protected:

		Handle<GameObject> mFollowing;
		float mDestinationX;
		float mDestinationY;

		float mLocationX;
		float mLocationY;


	};

}
}
