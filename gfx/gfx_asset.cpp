#include "gfx_asset.h"

#include "gfx_texture.h"
#include "gfx_engine.h"

#include "../logger.h"

#include <string>

using namespace std;

namespace am {
namespace gfx {

	// Asset methods
	Asset::Asset(GfxEngine *engine, const char *name) :
		GfxComponent(engine),
		mTexture(NULL),
		mName(name),
		mNumFramesX(1),
		mNumFramesY(1),
		mTotalFrames(1),
		mFrameRate(0.0f),
		mAnimationDirty(true)
	{

	}
	Asset::~Asset()
	{

	}

	void Asset::setTexture(const Texture *texture)
	{
		if (texture != NULL && texture->isLoaded())
		{
			mTexture = texture;
			mWindow.setValues(static_cast<float>(mTexture->getWidth()), 
				static_cast<float>(mTexture->getHeight()),
				0.0f, 1.0f, 0.0f, 1.0f);
		}
	}
	const Texture *Asset::getTexture() const
	{
		return mTexture;
	}

	// IGfxAsset methods
	const char *Asset::getName() const
	{
		return mName.c_str();
	}

	void Asset::setTextureWindow(const TextureWindow &window)
	{
		mWindow = window;
	}
	const TextureWindow &Asset::getTextureWindow() const
	{
		return mWindow;
	}

	void Asset::setNumFramesX(int num)
	{
		if (num < 1)
		{
			num = 1;
		}
		mNumFramesX = num;
		mAnimationDirty = true;
	}
	int Asset::getNumFramesX() const
	{
		return mNumFramesX;
	}
	void Asset::setNumFramesY(int num)
	{
		if (num < 1)
		{
			num = 1;
		}
		mNumFramesY = num;
		mAnimationDirty = true;
	}
	int Asset::getNumFramesY() const
	{
		return mNumFramesY;
	}

	void Asset::setTotalFrames(int frames)
	{
		if (frames < 1)
		{
			frames = 1;
		}
		mTotalFrames = frames;
		mAnimationDirty = true;
	}
	int Asset::getTotalFrames() const
	{
		return mTotalFrames;
	}

	void Asset::setFrameRate(float rate)
	{
		mFrameRate = rate;
	}
	float Asset::getFrameRate() const
	{
		return mFrameRate;
	}

	void Asset::setAnimation(int numX, int numY, int totalFrames, float frameRate)
	{
		if (numX < 1)
		{
			numX = 1;
		}
		mNumFramesX = numX;
		if (numY < 1)
		{
			numY = 1;
		}
		mNumFramesY = numY;
		if (totalFrames < 1)
		{
			totalFrames = 1;
		}
		mTotalFrames = totalFrames;
		mFrameRate = frameRate;
		mAnimationDirty = true;
	}


	const Asset::AnimationWindows &Asset::getAnimationWindows() const
	{
		return mAnimationWindows;
	}

	int Asset::loadDef(JsonValue value)
	{
		if (value.has("texture", JV_STR))
		{
			setTexture(mGfxEngine->getTexture(value["texture"].getCStr()));
			if (mTexture == NULL)
			{
				string errstr = "Unable to load texture (";
				errstr += value["texture"].getCStr();
				errstr += ") for asset '";
				errstr += mName.c_str();
				errstr += '\'';
				am_log("ASSET", errstr.c_str());
				return -1;
			}
		}
		else
		{
			return -1;
		}
		string str = "Loaded asset '";
		str += mName.c_str();
		str += '\'';
		am_log("ASSET", str.c_str());
		float textureWidth = static_cast<float>(mTexture->getWidth());
		float textureHeight = static_cast<float>(mTexture->getHeight());
		float width = textureWidth;
		float height = textureHeight;
		float leftX = 0.0f;
		float rightX = 1.0f;
		float topY = 0.0f;
		float bottomY = 1.0f;

		if (value.has("window", JV_OBJ))
		{
			JsonValue window = value["window"];
			float winLeftX = 0.0f;
			float winTopY = 0.0f;
			if (window.has("leftX", JV_FLOAT) || window.has("leftX", JV_INT))
			{
				winLeftX = window["leftX"].getFloat();
				leftX = winLeftX / textureWidth;
			}
			if (window.has("rightX", JV_FLOAT) || window.has("rightX", JV_INT))
			{
				float winRightX = window["rightX"].getFloat();
				width = winRightX - winLeftX;
				rightX = winRightX / textureWidth;
			}
			if (window.has("topY", JV_FLOAT) || window.has("topY", JV_INT))
			{
				winTopY = window["topY"].getFloat();
				topY = winTopY / textureHeight;
			}
			if (window.has("bottomY", JV_FLOAT) || window.has("bottomY", JV_INT))
			{
				float winBottomY = window["bottomY"].getFloat();
				height = winBottomY - winTopY;
				bottomY = winBottomY / textureHeight;
			}
		}

		mWindow.setValues(width, height, topY, bottomY, leftX, rightX);

		int framesX = 1;
		int framesY = 1;
		int totalFrames = 0;
		float frameRate = 1.0f;
		if (value.has("framesX", JV_INT))
		{
			framesX = value["framesX"].getInt();
		}
		if (value.has("framesY", JV_INT))
		{
			framesY = value["framesY"].getInt();
		}
		if (value.has("totalFrames", JV_INT))
		{
			totalFrames = value["totalFrames"].getInt();
		}
		if (totalFrames <= 0)
		{
			totalFrames = framesX * framesY;
		}

		if (value.has("frameRate", JV_INT) || value.has("frameRate", JV_FLOAT))
		{
			frameRate = value["frameRate"].getFloat();
		}
		setAnimation(framesX, framesY, totalFrames, frameRate);

		return 0;
	}

	void Asset::processAnimation()
	{
		if (!mAnimationDirty || mTexture == NULL)
		{
			return;
		}

		float textureWidth = static_cast<float>(mTexture->getWidth());
		float textureHeight = static_cast<float>(mTexture->getHeight());

		mWidth = textureWidth / static_cast<float>(mNumFramesX);
		mHeight = textureHeight / static_cast<float>(mNumFramesY);

		float frameWidth = textureWidth / static_cast<float>(mNumFramesX);
		float frameHeight = textureHeight / static_cast<float>(mNumFramesY);

		float uvWidth = frameWidth / textureWidth;
		float uvHeight = frameHeight / textureHeight;

		mAnimationWindows.clear();

		for (int y = 0; y < mNumFramesY; y++)
		{
			for (int x = 0; x < mNumFramesX; x++)
			{
				float xPos = static_cast<float>(x) * frameWidth;
				float yPos = static_cast<float>(y) * frameHeight;

				float uvXPos = xPos / textureWidth;
				float uvYPos = yPos / textureHeight;

				TextureWindow window;
				window.setValues(frameWidth, frameHeight,
					uvYPos, uvYPos + uvHeight,
					uvXPos, uvXPos + uvWidth);

				mWindow.createSubWindow(window);

				mAnimationWindows.push_back(window);
			}
		}

		mAnimationDirty = false;
	}

	float Asset::getWidth()
	{
		if (mAnimationDirty)
		{
			processAnimation();
		}
		return mWidth;
	}
	float Asset::getHeight()
	{
		if (mAnimationDirty)
		{
			processAnimation();
		}
		return mHeight;
	}

}
}
