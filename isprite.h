#pragma once

#include "irenderable.h"

namespace am {
namespace gfx {

	class ITexture;

	class ISprite : public IRenderable {
	public:

		virtual ITexture *getTexture() = 0;
		virtual void setTexture(ITexture *texture) = 0;

		virtual void setNumFramesX(int num) = 0;
		virtual int getNumFramesX() const = 0;
		virtual void setNumFramesY(int num) = 0;
		virtual int getNumFramesY() const = 0;

		virtual void setNumTotalFrames(int frames) = 0;
		virtual int getNumTotalFrames() const = 0;

		virtual void setCurrentFrame(int frame) = 0;
		virtual int getCurrentFrame() const = 0;

		virtual void setFrameRate(float rate) = 0;
		virtual float getFrameRate() const = 0;

		virtual void setFrameTime(float time) = 0;
		virtual float getFrameTime() const = 0;
	};

}
}
