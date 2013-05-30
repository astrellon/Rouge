#include "camera.h"

#include "gfx/gfx_engine.h"
#include <sfx/sfx_engine.h>
using namespace am::sfx;

namespace am {
namespace game {

	Camera::Camera() :
		mFollowing(NULL),
		mDestinationX(0.0f),
		mDestinationY(0.0f),
		mLocationX(0.0f),
		mLocationY(0.0f)
	{
	}
	Camera::~Camera()
	{
	}

	void Camera::followObject(GameObject *object)
	{
		mFollowing = object;
	}
	GameObject *Camera::getFollowing()
	{
		return mFollowing.get();
	}

	void Camera::setDestination(float x, float y)
	{
		mDestinationX = x;
		mDestinationY = y;
	}
	float Camera::getDestinationX()
	{
		return mDestinationX;
	}
	float Camera::getDestinationY()
	{
		return mDestinationY;
	}

	float Camera::getLocationX()
	{
		if (mFollowing.get())
		{
			return mFollowing->getLocationX() + mFollowing->getCameraOffsetX();
		}
		return mLocationX;
	}
	float Camera::getLocationY()
	{
		if (mFollowing.get())
		{
			return mFollowing->getLocationY() + mFollowing->getCameraOffsetY();
		}
		return mLocationY;
	}

	void Camera::update(float dt)
	{
		float locX = getLocationX();
		float locY = getLocationY();
		GfxEngine::getEngine()->setCameraLocation(locX, locY);
		stringstream ss;
		ss << "Setting sound loc to: " << locX << ", " << locY;
		//am_log("SFX", ss);
		SfxEngine::getEngine()->getListener().setPosition(locX, locY);
	}

}
}
