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

	class Screen;

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

		virtual void setScreen(Screen *screen);
		virtual Screen *getScreen();

	protected:

		float mGameLocationX;
		float mGameLocationY;

		float mCameraOffsetX;
		float mCameraOffsetY;

		Screen *mScreen;
	};

	typedef vector<Handle<GameObject> > ObjectList;

}
}
