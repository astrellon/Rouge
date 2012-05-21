#pragma once

#include "isprite.h"
#include "igl_renderable.h"

#include "transform.h"
#include "texture_window.h"

#include <vector>

using namespace std;

namespace am {
namespace gfx {

	class IGfxEngine;
	class ITexture;

	class GlGfxEngine;
	class GlTexture;

	class GlSprite : public ISprite, public IGlRenderable {
	public:
		// GlSprite methods
		GlSprite(GlGfxEngine *engine, GlTexture *texture);
		~GlSprite();

		GlTexture *getGlTexture();
		void setGlTexture(GlTexture *texture);

		// ISprite Methods
		virtual ITexture *getTexture();
		virtual void setTexture(ITexture *texture);

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

		// IRenderable methods
		virtual void render(float dt);
		virtual am::math::Transform &getTransform();

		virtual void setWidth(float width);
		virtual float getWidth() const;

		virtual void setHeight(float height);
		virtual float getHeight() const;

		// IGfxComponent methods
		virtual IGfxEngine *getGfxEngine();
		// IGlGfxComponent methods
		virtual GlGfxEngine *getGlGfxEngine();

	protected:

		GlTexture *mTexture;

		int mNumFramesX;
		int mNumFramesY;
		int mMaxFrames;
		int mCurrentFrame;

		float mFrameRate;
		float mCurrentTime;

		bool mAnimationDirty;
		vector<TextureWindow> mAnimationWindows;
		
		float mWidth;
		float mHeight;
		am::math::Transform mTransform;
		GlGfxEngine *mGfxEngine;

		void processAnimation();
	};

}
}
