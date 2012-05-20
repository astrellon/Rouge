#pragma once

#include "isprite.h"
#include "igl_renderable.h"

#include "transform.h"

namespace am {
namespace gfx {

	class IGfxEngine;
	class ITexture;

	class GlGfxEngine;
	class GlTexture;

	class GlSprite : public ISprite, public IGlRenderable {
	public:
		GlSprite(GlGfxEngine *engine, GlTexture *texture);
		~GlSprite();

		virtual ITexture *getTexture();
		GlTexture *getGlTexture();
		virtual void setTexture(ITexture *texture);
		void setGlTexture(GlTexture *texture);

		virtual void setNumFramesX(int num);
		virtual int getNumFramesX() const;
		virtual void setNumFramesY(int num);
		virtual int getNumFramesY() const;

		virtual void setNumTotalFrames(int frames);
		virtual int getNumTotalFrames() const;

		virtual void setCurrentFrame(int frame);
		virtual int getCurrentFrame() const;

		virtual void setFrameRate(float rate);
		virtual float getFrameRate() const;

		virtual void setFrameTime(float time);
		virtual float getFrameTime() const;

		virtual void render(float dt);
		virtual am::math::Transform &getTransform();

		virtual IGfxEngine *getGfxEngine();
		virtual GlGfxEngine *getGlGfxEngine();

	protected:

		GlTexture *mTexture;

		int mNumFramesX;
		int mNumFramesY;
		int mMaxFrames;
		int mCurrentFrame;

		float mFrameRate;
		float mCurrentTime;
		
		am::math::Transform mTransform;
		GlGfxEngine *mGfxEngine;
	};

}
}
