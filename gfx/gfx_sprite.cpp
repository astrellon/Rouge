#include "gfx_sprite.h"

#include "gfx_engine.h"
#include "gfx_texture.h"
#include "gfx_asset.h"
#include "gfx_layer.h"

#include "../logger.h"

namespace am {
namespace gfx {

	Sprite::Sprite() :
		Renderable(),
		mAsset(NULL),
		mCurrentFrame(0),
		mFrameRate(0.0f),
		mCurrentTime(0.0f),
		mScaleNineState(SCALE_NINE_NONE)
	{
		mColour.setColour(1.0f, 0.0f, 1.0f, 0.75f);
	}
	Sprite::Sprite(Asset *asset) :
		Renderable(),
		mAsset(asset),
		mCurrentFrame(0),
		mFrameRate(0.0f),
		mCurrentTime(0.0f)
	{
		if (asset)
		{
			mScaleNineState = asset->getScaleNineState();
		}
	}
	Sprite::Sprite(const char *assetName) :
		Renderable(),
		mCurrentFrame(0),
		mFrameRate(0.0f),
		mCurrentTime(0.0f),
		mAsset(NULL)
	{
		Asset *asset = NULL;
		asset = GfxEngine::getGfxEngine()->getAsset(assetName);
		
		if (asset)
		{
			mAsset = asset;
			mScaleNineState = mAsset->getScaleNineState();
		}
	}
	Sprite::~Sprite()
	{
	}

	Asset *Sprite::getAsset()
	{
		//return mAsset.get();
		return mAsset;
	}
	void Sprite::setAsset(Asset *asset)
	{
		mAsset = asset;
	}
	void Sprite::setAsset(const char *assetName)
	{
		mAsset = GfxEngine::getGfxEngine()->getAsset(assetName);
	}

	void Sprite::setCurrentFrame(int frame)
	{
		//if (!mAsset.get())
		if (!mAsset)
		{
			return;
		}
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

		if (mAsset == NULL || mAsset->getTexture() == NULL || !mAsset->getTexture()->isLoaded())
		{
			if (mColour.getAlpha() > 0.05f)
			{
				preRender(dt);
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
			if (getFrameRate() < 0.0001f && getFrameRate() > -0.0001f)
			{
				mCurrentTime = 0.0f;
			}
			else
			{
				mCurrentTime += dt * getFrameRate();
				float totalTime = static_cast<float>(mAsset->getTotalFrames());
				while (mCurrentTime > totalTime)
				{
					mCurrentTime -= totalTime;
				}
				while (mCurrentTime < 0.0f)
				{
					mCurrentTime += totalTime;
				}

				mCurrentFrame = static_cast<int>(mCurrentTime);
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

		const TextureWindow &win = mAsset->getAnimationWindows()[mCurrentFrame];

		renderTexture(win, width, height);

		glBindTexture(GL_TEXTURE_2D, 0);

		postRender(dt);
	}

	void Sprite::renderTexture(const TextureWindow &win, const float &width, const float &height)
	{
		if (mScaleNineState == SCALE_NINE_BOTH)
		{
			const ScaleNine &scaleNine = mAsset->getScaleNine();
			float rightX = width - (win.getWidth() - scaleNine.getRight());
			float bottomY = height - (win.getHeight() - scaleNine.getBottom());

			float leftU = scaleNine.getLeft() / mAsset->getTexture()->getWidth() + win.getLeftX();
			float rightU = scaleNine.getRight() / mAsset->getTexture()->getWidth() + win.getLeftX();
			float topV = scaleNine.getTop() / mAsset->getTexture()->getHeight() + win.getTopY();
			float bottomV = scaleNine.getBottom() / mAsset->getTexture()->getHeight() + win.getTopY();

			glBegin(GL_TRIANGLE_STRIP);

				// Top left
				glTexCoord2f(win.getLeftX(), win.getTopY());
				glVertex2f(0.0f, 0.0f);

				glTexCoord2f(win.getLeftX(), topV);
				glVertex2f(0.0f, scaleNine.getTop());

				glTexCoord2f(leftU, win.getTopY());
				glVertex2f(scaleNine.getLeft(), 0.0f);

				glTexCoord2f(leftU, topV);
				glVertex2f(scaleNine.getLeft(), scaleNine.getTop());

				// Top middle
				glTexCoord2f(rightU, win.getTopY());
				glVertex2f(rightX, 0.0f);

				glTexCoord2f(rightU, topV);
				glVertex2f(rightX, scaleNine.getTop());

				// Top right
				glTexCoord2f(win.getRightX(), win.getTopY());
				glVertex2f(width, 0.0f);

				glTexCoord2f(win.getRightX(), topV);
				glVertex2f(width, scaleNine.getTop());

			glEnd();

			glBegin(GL_TRIANGLE_STRIP);
				
				// Middle left
				glTexCoord2f(win.getLeftX(), topV);
				glVertex2f(0.0f, scaleNine.getTop());

				glTexCoord2f(win.getLeftX(), bottomV);
				glVertex2f(0.0f, bottomY);

				glTexCoord2f(leftU, topV);
				glVertex2f(scaleNine.getLeft(), scaleNine.getTop());

				glTexCoord2f(leftU, bottomV);
				glVertex2f(scaleNine.getLeft(), bottomY);

				// Middle middle
				glTexCoord2f(rightU, topV);
				glVertex2f(rightX, scaleNine.getTop());

				glTexCoord2f(rightU, bottomV);
				glVertex2f(rightX, bottomY);

				// Middle right
				glTexCoord2f(win.getRightX(), topV);
				glVertex2f(width, scaleNine.getTop());

				glTexCoord2f(win.getRightX(), bottomV);
				glVertex2f(width, bottomY);

			glEnd();

			glBegin(GL_TRIANGLE_STRIP);

				// Bottom left
				glTexCoord2f(win.getLeftX(), bottomV);
				glVertex2f(0.0f, bottomY);

				glTexCoord2f(win.getLeftX(), win.getBottomY());
				glVertex2f(0.0f, height);

				glTexCoord2f(leftU, bottomV);
				glVertex2f(scaleNine.getLeft(), bottomY);

				glTexCoord2f(leftU, win.getBottomY());
				glVertex2f(scaleNine.getLeft(), height);

				// Bottom middle
				glTexCoord2f(rightU, bottomV);
				glVertex2f(rightX, bottomY);

				glTexCoord2f(rightU, win.getBottomY());
				glVertex2f(rightX, height);

				// Bottom right
				glTexCoord2f(win.getRightX(), bottomV);
				glVertex2f(width, bottomY);

				glTexCoord2f(win.getRightX(), win.getBottomY());
				glVertex2f(width, height);

			glEnd();
		}
		else
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
	}

	float Sprite::getWidth()
	{
		if (mWidth == 0 && mAsset)
		{
			return mAsset->getWidth();
		}
		return mWidth;
	}
	float Sprite::getHeight()
	{
		if (mHeight == 0 && mAsset)
		{
			return mAsset->getHeight();
		}
		return mHeight;
	}

	void Sprite::setScaleNineState(ScaleNineState state)
	{
		mScaleNineState = state;
	}
	ScaleNineState Sprite::getScaleNineState() const
	{
		return mScaleNineState;
	}

}
}
