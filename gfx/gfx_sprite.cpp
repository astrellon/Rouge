#include "gfx_sprite.h"

#include "gfx_engine.h"
#include "gfx_texture.h"
#include "gfx_asset.h"
#include "gfx_layer.h"

#include <log/logger.h>

#include <game/loading_state.h>

#include <util/data_string.h>
#include <util/data_number.h>
#include <util/data_boolean.h>
#include <util/data_table.h>

namespace am {
namespace gfx {

	const int Sprite::LUA_ID = 0x13;
	const char *Sprite::LUA_TABLENAME = "am_gfx_Sprite";

	Sprite::Sprite() :
		Renderable(),
		mAsset(NULL),
		mCurrentFrame(0),
		mFrameRate(0.0f),
		mCurrentTime(0.0f),
		mScaleNineState(SCALE_NINE_NONE)
	{
		//mColour.setColour(1.0f, 0.0f, 1.0f, 0.75f);
	}
	Sprite::Sprite(const Sprite &copy) :
		Renderable(copy),
		mAsset(copy.mAsset),
		mCurrentFrame(copy.mCurrentFrame),
		mFrameRate(copy.mFrameRate),
		mCurrentTime(copy.mCurrentTime),
		mScaleNineState(copy.mScaleNineState)
	{

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
		asset = GfxEngine::getEngine()->getAssetLua(assetName);
		
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
		return mAsset;
	}
	void Sprite::setAsset(Asset *asset)
	{
		mAsset = asset;
	}
	void Sprite::setAsset(const char *assetName)
	{
		mAsset = GfxEngine::getEngine()->getAssetLua(assetName);
	}

	void Sprite::setCurrentFrame(int frame)
	{
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

	data::IData *Sprite::serialise()
	{
		data::Table *output = new data::Table();
		if (mAsset)
		{
			output->at("asset", mAsset->getName());
		}
		if (mWidth != 0 || !mAsset)
		{
			output->at("width", mWidth);
		}
		if (mHeight != 0 || !mAsset)
		{
			output->at("height", mHeight);
		}
		if (mAsset && mAsset->getTotalFrames() > 1)
		{
			output->at("currentFrame", mCurrentFrame);
			output->at("frameRate", mFrameRate);
			output->at("currentTime", mCurrentTime);
		}
		output->at("scaleNineState", ScaleNine::getStateName(mScaleNineState));
		return output;
	}
	void Sprite::deserialise(am::game::LoadingState *state, data::IData *data)
	{
		Handle<data::Table> dataMap(dynamic_cast<data::Table *>(data));
		if (!dataMap)
		{
			stringstream ss;
			ss << "Unable to load sprite from '" << data->typeName();
		}

		Handle<data::String> str(dataMap->at<data::String>("asset"));
		if (str)
		{
			setAsset(str->string());
		}

		Handle<data::Number> num(dataMap->at<data::Number>("width"));
		if (num)
		{
			mWidth = num->value<float>();
		}
		num = dataMap->at<data::Number>("height");
		if (num)
		{
			mHeight = num->value<float>();
		}

		num = dataMap->at<data::Number>("currentFrame");
		if (num)
		{
			mCurrentFrame = num->valuei();
		}
		num = dataMap->at<data::Number>("frameRate");
		if (num)
		{
			mFrameRate = num->value<float>();
		}
		num = dataMap->at<data::Number>("currentTime");
		if (num)
		{
			mCurrentTime = num->value<float>();
		}

		str = dataMap->at<data::String>("scaleNineState");
		if (str)
		{
			setScaleNineState(ScaleNine::getState(str->string()));
		}
	}

	void Sprite::render(float dt)
	{
		if (!mVisible)
		{
			return;
		}

		if (mAsset == NULL || mAsset->getTexture() == NULL || !mAsset->getTexture()->isLoaded())
		{
			if (!mGfxComponent || (mGfxComponent && mGfxComponent->getColour().getAlpha() > 0.05f))
			{
				preRender(dt);
				Texture::bindTexture(0);

				if (mGfxComponent)
				{
					mGfxComponent->getColour().applyColour();
				}
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
		
		updateSprite(dt);
		renderSprite();

		postRender(dt);
	}

	void Sprite::updateSprite(float dt)
	{
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
	}

	void Sprite::renderSprite()
	{
		float width = getWidth();
		float height = getHeight();
		
		//glBindTexture(GL_TEXTURE_2D, mAsset->getTexture()->getTextureId());
		mAsset->getTexture()->bindTexture();

		const TextureWindow &win = mAsset->getAnimationWindows()[mCurrentFrame];

		renderTexture(win, width, height);
		//glBindTexture(GL_TEXTURE_2D, 0);
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
		else if (mScaleNineState == SCALE_NINE_VERTICAL)
		{
			const ScaleNine &scaleNine = mAsset->getScaleNine();
			float bottomY = height - (win.getHeight() - scaleNine.getBottom());

			float topV = scaleNine.getTop() / mAsset->getTexture()->getHeight() + win.getTopY();
			float bottomV = scaleNine.getBottom() / mAsset->getTexture()->getHeight() + win.getTopY();

			glBegin(GL_TRIANGLE_STRIP);

				// Top
				glTexCoord2f(win.getLeftX(), win.getTopY());
				glVertex2f(0.0f, 0.0f);

				glTexCoord2f(win.getRightX(), win.getTopY());
				glVertex2f(width, 0.0f);

				glTexCoord2f(win.getLeftX(), topV);
				glVertex2f(0.0f, scaleNine.getTop());

				glTexCoord2f(win.getRightX(), topV);
				glVertex2f(width, scaleNine.getTop());

				// Middle
				glTexCoord2f(win.getLeftX(), bottomV);
				glVertex2f(0.0f, bottomY);

				glTexCoord2f(win.getRightX(), bottomV);
				glVertex2f(width, bottomY);

				// Bottom
				glTexCoord2f(win.getLeftX(), win.getBottomY());
				glVertex2f(0.0f, height);

				glTexCoord2f(win.getRightX(), win.getBottomY());
				glVertex2f(width, height);

			glEnd();
		}
		else
		{
			float minU = 0.0f;
			float maxU = win.getRightX();
			if (mAsset->getRepeatX()) 
			{
				maxU = width / static_cast<float>(mAsset->getTexture()->getWidth());
			}
			else
			{
				minU = win.getLeftX();
			}

			float minV = 0.0f;
			float maxV = win.getBottomY();
			if (mAsset->getRepeatY())
			{
				maxV = height / static_cast<float>(mAsset->getTexture()->getHeight());
			}
			else
			{
				minV = win.getTopY();
			}
			//glBegin(GL_TRIANGLE_STRIP);
			glBegin(GL_QUADS);

				glTexCoord2f(minU, minV);
				glVertex2f(0, 0);

				glTexCoord2f(maxU, minV);
				glVertex2f(width, 0);

				glTexCoord2f(maxU, maxV);
				glVertex2f(width, height);

				glTexCoord2f(minU, maxV);
				glVertex2f(0, height);
			
				

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
