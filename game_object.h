#pragma once

#include "gfx/gfx_layer.h"
using namespace am::gfx;

#include "vector.h"
using namespace am::math;

namespace am {
namespace base {

	class GameObject : public Layer {
	public:
		GameObject();
		~GameObject();

		//virtual void setGameLocation(const Vector4f &pos, bool setDraw = true);
		virtual void setGameLocation(float x, float y, bool setDraw = true);
		virtual float getGameLocationX() const;
		virtual float getGameLocationY() const;

	protected:

		//Vector4f mGameLocation;
		float mGameLocationX;
		float mGameLocationY;
	};

}
}
