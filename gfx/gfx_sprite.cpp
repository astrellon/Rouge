#include "gfx_sprite.h"

#include "gfx_engine.h"
#include "gfx_texture.h"
#include "gfx_asset.h"
#include "gfx_layer.h"

namespace am {
namespace gfx {

	Sprite::Sprite(GfxEngine *engine) :
		Renderable(engine),
		mAsset(NULL),
		mCurrentFrame(0),
		mFrameRate(0.0f),
		mCurrentTime(0.0f)
	{
		mColour.setColour(1.0f, 0.0f, 1.0f, 0.75f);
	}
	Sprite::Sprite(GfxEngine *engine, Asset *asset) :
		Renderable(engine),
		mAsset(asset),
		mCurrentFrame(0),
		mFrameRate(0.0f),
		mCurrentTime(0.0f)
	{

	}
	Sprite::Sprite(GfxEngine *engine, const char *assetName) :
		Renderable(engine),
		mCurrentFrame(0),
		mFrameRate(0.0f),
		mCurrentTime(0.0f)
	{
		mAsset = engine->getAsset(assetName);
	}
	Sprite::~Sprite()
	{
	}

	Asset *Sprite::getAsset()
	{
		return mAsset.get();
	}
	void Sprite::setAsset(Asset *asset)
	{
		mAsset = asset;
	}
	void Sprite::setAsset(const char *assetName)
	{
		mAsset = mGfxEngine->getAsset(assetName);
	}

	void Sprite::setCurrentFrame(int frame)
	{
		if (frame >= mAsset->getTotalFrames())
		{
			frame = mAsset->getTotalFrames() - 1;
		}
		if (frame != mCurrentFrame) 
		{
			mCurrentFrame = frame;
			if (mFrameRate < 0.0001f)
			{
				mCurrentTime = 0.0f;
			}
			else
			{
				mCurrentTime = static_cast<float>(mCurrentFrame) / static_cast<float>(mAsset->getTotalFrames()) / 
					getFrameRate();
			}
		}
	}
	int Sprite::getCurrentFrame() const
	{
		return mCurrentFrame;
	}

	void Sprite::setFrameRate(float rate)
	{
		if (rate != mFrameRate)
		{
			mFrameRate = rate;
			if (rate < 0.0001f)
			{
				mCurrentTime = 0.0f;
			}
			else
			{
				mCurrentTime = static_cast<float>(mCurrentFrame) / static_cast<float>(mAsset->getTotalFrames()) / 
					mFrameRate;
			}
		}
	}
	float Sprite::getFrameRate() const
	{
		if (mFrameRate == 0)
		{
			return mAsset->getFrameRate();
		}
		return mFrameRate;
	}

	void Sprite::setFrameTime(float time)
	{
		if (time != mCurrentTime)
		{
			mCurrentTime = time;
			setCurrentFrame(static_cast<int>(mCurrentTime * mFrameRate));
		}
	}
	float Sprite::getFrameTime() const
	{
		return mCurrentTime;
	}

	void Sprite::render(float dt)
	{
		if (!mVisible)
		{
			return;
		}

		if (mAsset.get() == NULL || mAsset->getTexture() == NULL || !mAsset->getTexture()->isLoaded())
		{
			if (mColour.getAlpha() > 0.05f)
			{
				preRender(dt);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				glBindTexture(GL_TEXTURE_2D, 0);

				mColour.applyColour();
				glBegin(GL_QUADS);
					glVertex2f(0.0f, 0.0f);
					glVertex2f(getWidth(), 0.0f);
					glVertex2f(getWidth(), getHeight());
					glVertex2f(0.0f, getHeight());
				glEnd();

				postRender(dt);
			}

			return;
		}

		preRender(dt);
		
		mAsset->processAnimation();

		if (mAsset->getTotalFrames() > 1)
		{
			if (getFrameRate() < 0.0001f)
			{
				mCurrentTime = 0.0f;
			}
			else
			{
				mCurrentTime += dt;
				float totalTime = static_cast<float>(mAsset->getTotalFrames()) / getFrameRate();
				while (mCurrentTime > totalTime)
				{
					mCurrentTime -= totalTime;
				}
				while (mCurrentTime < 0.0f)
				{
					mCurrentTime += totalTime;
				}

				mCurrentFrame = static_cast<int>(mCurrentTime * getFrameRate());
				if (mCurrentFrame >= mAsset->getTotalFrames())
				{
					mCurrentFrame = mAsset->getTotalFrames() - 1;
				}
				if (mCurrentFrame < 0)
				{
					mCurrentFrame = 0;
				}
			}
		}
		else
		{
			mCurrentFrame = 0;
			mCurrentTime = 0.0f;
		}

		float width = getWidth();
		float height = getHeight();
		
		glBindTexture(GL_TEXTURE_2D, mAsset->getTexture()->getTextureId());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		const TextureWindow &win = mAsset->getAnimationWindows()[mCurrentFrame];

		renderTexture(win, width, height);

		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);

		postRender(dt);
	}

	void Sprite::renderTexture(const TextureWindow &win, const float &width, const float &height)
	{
		glBegin(GL_TRIANGLE_STRIP);

			glTexCoord2f(win.getLeftX(), win.getTopY());
			glVertex2f(0, 0);

			glTexCoord2f(win.getRightX(), win.getTopY());
			glVertex2f(width, 0);

			glTexCoord2f(win.getLeftX(), win.getBottomY());
			glVertex2f(0, height);
			
			glTexCoord2f(win.getRightX(), win.getBottomY());
			glVertex2f(width, height);

		glEnd();
	}

	float Sprite::getWidth()
	{
		if (mWidth == 0)
		{
			return mAsset->getWidth();
		}
		return mWidth;
	}
	float Sprite::getHeight()
	{
		if (mHeight == 0)
		{
			return mAsset->getHeight();
		}
		return mHeight;
	}	

}
}
