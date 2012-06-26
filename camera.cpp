#include "camera.h"

#include "gfx/gfx_engine.h"

namespace am {
namespace base {

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
		GfxEngine::getEngine()->setCameraLocation(getLocationX(), getLocationY());
	}

}
}
