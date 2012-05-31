#include "gfx_sprite.h"

#include "gfx_engine.h"
#include "gfx_texture.h"
#include "gfx_asset.h"
#include "gfx_layer.h"

namespace am {
namespace gfx {

	Sprite::Sprite(GfxEngine *engine, Asset *asset) :
		Renderable(engine),
		mAsset(asset),
		mNumFramesX(1),
		mNumFramesY(1),
		mMaxFrames(1),
		mCurrentFrame(0),
		mFrameRate(0.0f),
		mCurrentTime(0.0f),
		mAnimationDirty(true)
	{
	}
	Sprite::Sprite(GfxEngine *engine, const char *assetName) :
		Renderable(engine),
		mNumFramesX(1),
		mNumFramesY(1),
		mMaxFrames(1),
		mCurrentFrame(0),
		mFrameRate(0.0f),
		mCurrentTime(0.0f),
		mAnimationDirty(true)
	{
		mAsset = engine->getAsset(assetName);
	}
	Sprite::~Sprite()
	{
	}

	Asset *Sprite::getAsset()
	{
		return mAsset;
	}
	void Sprite::setAsset(Asset *asset)
	{
		mAsset = asset;
		mAnimationDirty = true;
	}

	void Sprite::setNumFramesX(int num)
	{
		if (num < 1)
		{
			num = 1;
		}
		mNumFramesX = num;
		mAnimationDirty = true;
	}
	int Sprite::getNumFramesX() const
	{
		return mNumFramesX;
	}
	void Sprite::setNumFramesY(int num)
	{
		if (num < 1)
		{
			num = 1;
		}
		mNumFramesY = num;
		mAnimationDirty = true;
	}
	int Sprite::getNumFramesY() const
	{
		return mNumFramesY;
	}

	void Sprite::setNumTotalFrames(int frames)
	{
		if (frames < 1)
		{
			frames = 1;
		}
		mMaxFrames = frames;
		mAnimationDirty = true;
	}
	int Sprite::getNumTotalFrames() const
	{
		return mMaxFrames;
	}

	void Sprite::setCurrentFrame(int frame)
	{
		mCurrentFrame = frame;
	}
	int Sprite::getCurrentFrame() const
	{
		return mCurrentFrame;
	}

	void Sprite::setFrameRate(float rate)
	{
		mFrameRate = rate;
	}
	float Sprite::getFrameRate() const
	{
		return mFrameRate;
	}

	void Sprite::setFrameTime(float time)
	{
		mCurrentTime = time;
	}
	float Sprite::getFrameTime() const
	{
		return mCurrentTime;
	}

	void Sprite::render(float dt)
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
			width = static_cast<float>(mAsset->getTexture()->getWidth());
		}

		float height = mHeight;
		if (height == 0)
		{
			height = static_cast<float>(mAsset->getTexture()->getHeight());
		}

		if (mAnimationDirty)
		{
			processAnimation();
		}
		
		glBindTexture(GL_TEXTURE_2D, mAsset->getTexture()->getTextureId());

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
	
	void Sprite::processAnimation()
	{
		float textureWidth = static_cast<float>(mAsset->getTexture()->getWidth());
		float textureHeight = static_cast<float>(mAsset->getTexture()->getHeight());

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

			mAsset->getTextureWindow().createSubWindow(window);

			mAnimationWindows.push_back(window);
		}

		mAnimationDirty = false;
	}

}
}
