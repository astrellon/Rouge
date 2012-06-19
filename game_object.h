#pragma once

#include "gfx/gfx_layer.h"
using namespace am::gfx;

#include "vector.h"
using namespace am::math;

#include "handle.h"
using namespace am::util;

#include <vector>
using namespace std;

namespace am {
namespace base {

	class GameObject : public Layer {
	public:
		GameObject();
		~GameObject();

		virtual void setGameLocation(float x, float y, bool setDraw = true);
		virtual float getGameLocationX() const;
		virtual float getGameLocationY() const;

		virtual void update(float dt) = 0;

		virtual void setCameraOffset(float x, float y);
		virtual float getCameraOffsetX() const;
		virtual float getCameraOffsetY() const;

	protected:

		float mGameLocationX;
		float mGameLocationY;

		float mCameraOffsetX;
		float mCameraOffsetY;
	};

	typedef vector<Handle<GameObject> > ObjectList;

}
}
