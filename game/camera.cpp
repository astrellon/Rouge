#include "camera.h"

#include <gfx/gfx_engine.h>

#include <sfx/sfx_engine.h>
using namespace am::sfx;

#include <game/engine.h>
using namespace am::game;

namespace am {
namespace game {

	Camera::Camera() :
		mFollowing(nullptr),
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
		return mFollowing;
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
		if (mFollowing)
		{
			if (mFollowing->isFixedToGrid())
			{
				return mFollowing->getGridLocationX() * Engine::getEngine()->getGridSize() + mFollowing->getCameraOffsetX();
			}
			return mFollowing->getLocationX() + mFollowing->getCameraOffsetX();
		}
		return mLocationX;
	}
	float Camera::getLocationY()
	{
		if (mFollowing)
		{
			if (mFollowing->isFixedToGrid())
			{
				return mFollowing->getGridLocationY() * Engine::getEngine()->getGridSize() + mFollowing->getCameraOffsetY();
			}
			return mFollowing->getLocationY() + mFollowing->getCameraOffsetY();
		}
		return mLocationY;
	}

	void Camera::update(float dt)
	{
		if (!mFollowing)
		{
			//mLocationX += (mDestinationX - mLocationX) * 0.1f;
			//mLocationY += (mDestinationY - mLocationY) * 0.1f;
			mLocationX = mDestinationX;
			mLocationY = mDestinationY;
		}
		float locX = getLocationX();
		float locY = getLocationY();
		GfxEngine::getEngine()->setCameraLocation(locX, locY);
		std::stringstream ss;
		ss << "Setting sound loc to: " << locX << ", " << locY;
		//am_log("SFX", ss);
		SfxEngine::getEngine()->getListener().setPosition(locX, locY);
	}

}
}
