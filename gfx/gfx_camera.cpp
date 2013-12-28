#include "gfx_camera.h"

#include <gfx/gfx_engine.h>
#include <gfx/gfx_renderable.h>

#include <sfx/sfx_engine.h>

#include <game/engine.h>

#include <gl.h>

namespace am {
namespace gfx {

	Camera::Camera() :
		mDestinationX(0.0f),
		mDestinationY(0.0f),
		mLocationX(0.0f),
		mLocationY(0.0f),
        mNear(0),
        mFar(1000.0f),
        mFov(65.0f),
        mZoom(1.0f),
        mPerspective(false)
    {
	}
	Camera::~Camera()
	{
	}

	void Camera::followObject(Renderable *object)
	{
		mFollowing = object;
	}
	Renderable *Camera::getFollowing()
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
			/*if (mFollowing->isFixedToGrid())
			{
				return mFollowing->getGridLocationX() * Engine::getEngine()->getGridSize() + mFollowing->getCameraOffsetX();
			}
			return mFollowing->getLocationX() + mFollowing->getCameraOffsetX();*/
            return mFollowing->getPositionX();
		}
		return mLocationX;
	}
	float Camera::getLocationY()
	{
		if (mFollowing)
		{
			/*if (mFollowing->isFixedToGrid())
			{
				return mFollowing->getGridLocationY() * Engine::getEngine()->getGridSize() + mFollowing->getCameraOffsetY();
			}
			return mFollowing->getLocationY() + mFollowing->getCameraOffsetY();*/
            return mFollowing->getPositionY();
		}
		return mLocationY;
	}

    void Camera::setPerspective(bool persp)
    {
        mPerspective = persp;
    }
    bool Camera::isPerspective() const
    {
        return mPerspective;
    }

    void Camera::setNear(float near)
    {
        mNear = near;
    }
    float Camera::getNear() const
    {
        return mNear;
    }

    void Camera::setFar(float far)
    {
        mFar = far;
    }
    float Camera::getFar() const
    {
        return mFar;
    }

    void Camera::setOrthographicZoom(float zoom)
    {
        mZoom = zoom;
    }
    float Camera::getOrthographicZoom() const
    {
        return mZoom;
    }

    void Camera::setFieldOfView(float fov)
    {
        mFov = fov;
    }
    float Camera::getFieldOfView() const
    {
        return mFov;
    }

    math::Transform &Camera::getTransform()
    {
        return mTransform;
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
		am::gfx::GfxEngine::getEngine()->setCameraLocation(locX, locY);
		am::sfx::SfxEngine::getEngine()->getListener().setPosition(locX, locY);
	}
    void Camera::apply(int resolutionWidth, int resolutionHeight)
    {
        if (mPerspective)
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            float aspect = static_cast<float>(resolutionWidth) / static_cast<float>(resolutionHeight);
            float fH = tan(static_cast<float>(mFov / 360.0f * 3.14159f)) * mNear;
            float fW = fH * aspect;
            glFrustum( -fW, fW, -fH, fH, mNear, mFar);

            glMatrixMode(GL_MODELVIEW);
        }
        else
        {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();

            glOrtho(0, resolutionWidth, resolutionHeight, 0, mNear, mFar);
            glMatrixMode(GL_MODELVIEW);

        }
        //glTranslatef(resolutionWidth / 2, resolutionHeight / 2, 0.0f);
        //glMultMatrixf(mTransform.data());
    }

}
}
