#include "gl_sprite.h"

#include "gl_gfx_engine.h"
#include "gl_texture.h"

namespace am {
namespace gfx {

	GlSprite::GlSprite(GlGfxEngine *engine, GlTexture *texture) :
		mGfxEngine(engine),
		mTexture(texture),
		mNumFramesX(1),
		mNumFramesY(1),
		mMaxFrames(1),
		mCurrentFrame(0),
		mFrameRate(0.0f),
		mCurrentTime(0.0f)
	{

	}
	GlSprite::~GlSprite()
	{
	}

	ITexture *GlSprite::getTexture()
	{
		return mTexture;
	}
	GlTexture *GlSprite::getGlTexture()
	{
		return mTexture;
	}
	void GlSprite::setTexture(ITexture *texture)
	{
		GlTexture *glTexture = dynamic_cast<GlTexture *>(texture);
		if (glTexture != NULL)
		{
			mTexture = glTexture;
		}
	}
	void GlSprite::setGlTexture(GlTexture *texture)
	{
		mTexture = texture;
	}

	void GlSprite::setNumFramesX(int num)
	{
		if (num < 1)
		{
			num = 1;
		}
		mNumFramesX = num;
	}
	int GlSprite::getNumFramesX() const
	{
		return mNumFramesX;
	}
	void GlSprite::setNumFramesY(int num)
	{
		if (num < 1)
		{
			num = 1;
		}
		mNumFramesY = num;
	}
	int GlSprite::getNumFramesY() const
	{
		return mNumFramesY;
	}

	void GlSprite::setNumTotalFrames(int frames)
	{
		if (frames < 1)
		{
			frames = 1;
		}
		mMaxFrames = frames;
	}
	int GlSprite::getNumTotalFrames() const
	{
		return mMaxFrames;
	}

	void GlSprite::setCurrentFrame(int frame)
	{
		mCurrentFrame = frame;
	}
	int GlSprite::getCurrentFrame() const
	{
		return mCurrentFrame;
	}

	void GlSprite::setFrameRate(float rate)
	{
		mFrameRate = rate;
	}
	float GlSprite::getFrameRate() const
	{
		return mFrameRate;
	}

	void GlSprite::setFrameTime(float time)
	{
		mCurrentTime = time;
	}
	float GlSprite::getFrameTime() const
	{
		return mCurrentTime;
	}

	void GlSprite::render(float dt)
	{

	}
	am::math::Transform &GlSprite::getTransform()
	{
		return mTransform;
	}

	IGfxEngine *GlSprite::getGfxEngine()
	{
		return mGfxEngine;
	}
	GlGfxEngine *GlSprite::getGlGfxEngine()
	{
		return mGfxEngine;
	}

}
}
