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
		mCurrentTime(0.0f),
		mWidth(0.0f),
		mHeight(0.0f),
		mAnimationDirty(true)
	{
		mTransform.setUpDirection(am::math::Transform::REF_FORWARD);
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
			mAnimationDirty = true;
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
		mAnimationDirty = true;
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
		mAnimationDirty = true;
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
		mAnimationDirty = true;
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
		glPushMatrix();
		glMultMatrixf(mTransform.data());

		if (mMaxFrames > 1)
		{
			mCurrentTime += dt;
			float totalTime = static_cast<float>(mMaxFrames) / mFrameRate;
			while (mCurrentTime > totalTime)
			{
				mCurrentTime -= totalTime;
			}
			mCurrentFrame = static_cast<int>(mMaxFrames * mCurrentTime / totalTime);
		}

		float width = mWidth;
		if (width == 0)
		{
			width = static_cast<float>(mTexture->getWidth());
		}

		float height = mHeight;
		if (height == 0)
		{
			height = static_cast<float>(mTexture->getHeight());
		}

		if (mAnimationDirty)
		{
			processAnimation();
		}
		
		glBindTexture(GL_TEXTURE_2D, mTexture->getTextureId());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		const TextureWindow &win = mAnimationWindows[mCurrentFrame];

		glBegin(GL_QUADS);

			glTexCoord2f(win.getLeftX(), win.getTopY());
			glVertex2f(0, 0);

			glTexCoord2f(win.getRightX(), win.getTopY());
			glVertex2f(width, 0);

			glTexCoord2f(win.getRightX(), win.getBottomY());
			glVertex2f(width, height);

			glTexCoord2f(win.getLeftX(), win.getBottomY());
			glVertex2f(0, height);

		glEnd();

		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
	}
	am::math::Transform &GlSprite::getTransform()
	{
		return mTransform;
	}

	void GlSprite::setWidth(float width)
	{
		mWidth = width;
	}
	float GlSprite::getWidth() const
	{
		return mWidth;
	}

	void GlSprite::setHeight(float height)
	{
		mHeight = height;
	}
	float GlSprite::getHeight() const
	{
		return mHeight;
	}

	IGfxEngine *GlSprite::getGfxEngine()
	{
		return mGfxEngine;
	}
	GlGfxEngine *GlSprite::getGlGfxEngine()
	{
		return mGfxEngine;
	}

	void GlSprite::processAnimation()
	{
		float textureWidth = mTexture->getWidth();
		float textureHeight = mTexture->getHeight();

		float frameWidth = textureWidth / static_cast<float>(mNumFramesX);
		float frameHeight = textureHeight / static_cast<float>(mNumFramesY);

		float uvWidth = frameWidth / textureWidth;
		float uvHeight = frameHeight / textureHeight;

		mAnimationWindows.clear();

		for (int i = 0; i < mMaxFrames; i++)
		{
			float xPos = static_cast<float>(i / mNumFramesX) * frameWidth;
			float yPos = static_cast<float>(i % mNumFramesY) * frameHeight;

			float uvXPos = xPos / textureWidth;
			float uvYPos = yPos / textureHeight;

			TextureWindow window;
			window.setValues(frameWidth, frameHeight,
				uvXPos, uvXPos + uvWidth,
				uvYPos, uvYPos + uvHeight);

			mAnimationWindows.push_back(window);
		}

		mAnimationDirty = false;
	}
}
}
