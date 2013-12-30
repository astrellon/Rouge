#pragma once

#include <base/handle.h>
#include <base/imanaged.h>

#include <math/transform.h>

#include "gfx_renderable.h"

namespace am {
namespace gfx {

	class Camera : public base::IManaged 
	{
	public:
		Camera();
		~Camera();

		void followObject(Renderable *object);
		Renderable *getFollowing();

		void setDestination(float x, float y);
		float getDestinationX();
		float getDestinationY();

		float getLocationX();
		float getLocationY();

        void setPerspective(bool persp);
        bool isPerspective() const;

        void setNear(float near);
        float getNear() const;

        void setFar(float far);
        float getFar() const;

        void setOrthographicZoom(float zoom);
        float getOrthographicZoom() const;

        void setFieldOfView(float fov);
        float getFieldOfView() const;

        math::Transform &getTransform();

		void update(float dt);
        void apply(int resolutionWidth, int resolutionHeight);

	protected:

        base::Handle<Renderable> mFollowing;
		float mDestinationX;
		float mDestinationY;

		float mLocationX;
		float mLocationY;

        float mNear;
        float mFar;

        float mFov;
        float mZoom;

        bool mPerspective;
        math::Transform mTransform;


	};

}
}
