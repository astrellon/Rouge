#include "gfx_asset.h"

#include "gfx_texture.h"
#include "gfx_engine.h"

#include <log/logger.h>

#include <string>

using namespace std;

namespace am {
namespace gfx {

	// Asset methods
	Asset::Asset(const char *name) :
		IManaged(),
		mTexture(NULL),
		mName(name),
		mNumFramesX(1),
		mNumFramesY(1),
		mTotalFrames(1),
		mFrameRate(0.0f),
		mAnimationDirty(true),
		mScaleNineState(SCALE_NINE_NONE),
		mRepeatX(false),
		mRepeatY(false)
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

	int Asset::loadDef(LuaState &lua, bool reload)
	{
		if (!lua_istable(lua, -1))
		{
			return -1;
		}
		if (lua.isTableString("texture"))
		{
			const char *texture = lua_tostring(lua, -1);
			lua.pop(1);
			const char *currentTexture = NULL;
			if (mTexture != NULL)
			{
				currentTexture = mTexture->getFilename();
			}
			if (reload && strcmp(texture, currentTexture) == 0)
			{
				if (!GfxEngine::getEngine()->reloadTexture(texture))
				{
					stringstream errss;
					errss << "Unable to reload texture ("<< texture << ") for asset '"
						<< mName.c_str() << '\'';
					am_log("ASSET", errss);
					return -1;
				}
			}
			else
			{
				setTexture(GfxEngine::getEngine()->getTexture(texture));
				if (mTexture == NULL)
				{
					stringstream errss;
					errss << "Unable to load texture ("<< texture << ") for asset '"
						<< mName.c_str() << '\'';
					am_log("ASSET", errss);
					return -1;
				}
			}
		}
		else
		{
			return -1;
		}
		{
			stringstream ss;
			ss << "Loading asset '" << mName.c_str() << '\'';
			am_log("ASSET", ss);
		}
		float textureWidth = static_cast<float>(mTexture->getWidth());
		float textureHeight = static_cast<float>(mTexture->getHeight());
		float width = textureWidth;
		float height = textureHeight;
		float leftX = 0.0f;
		float rightX = 1.0f;
		float topY = 0.0f;
		float bottomY = 1.0f;

		if (lua.isTableTable("window"))
		{
			float winLeftX = 0.0f;
			float winTopY = 0.0f;
			
			if (lua.isTableNumber("leftX"))
			{
				winLeftX = lua.toNumber();
				leftX = winLeftX / textureWidth;
			}
			if (lua.isTableNumber("rightX"))
			{
				float winRightX = lua.toNumber();
				width = winRightX - winLeftX;
				rightX = winRightX / textureWidth;
			}

			if (lua.isTableNumber("topY"))
			{
				winTopY = lua.toNumber();
				topY = winTopY / textureHeight;
			}

			if (lua.isTableNumber("bottomY"))
			{
				float winBottomY = lua.toNumber();
				height = winBottomY - winTopY;
				bottomY = winBottomY / textureHeight;
			}
			lua.pop(1);
		}

		mWindow.setValues(width, height, topY, bottomY, leftX, rightX);

		int framesX = 1;
		int framesY = 1;
		int totalFrames = 0;
		float frameRate = 1.0f;
		if (lua.isTableNumber("framesX"))
		{
			framesX = lua.toInteger();
		}
		if (lua.isTableNumber("framesY"))
		{
			framesY = lua.toInteger();
		}
		if (lua.isTableNumber("totalFrames"))
		{
			totalFrames = lua.toInteger();
		}
		if (totalFrames <= 0)
		{
			totalFrames = framesX * framesY;
		}

		if (lua.isTableNumber("frameRate"))
		{
			frameRate = lua.toNumber();
		}
		setAnimation(framesX, framesY, totalFrames, frameRate);

		if (lua.isTableTable("scaleNine"))
		{
			float left = -1.0f;
			float right = -1.0f;

			float top = -1.0f;
			float bottom = -1.0f;

			if (lua.isTableNumber("left"))
			{
				left = lua.toNumber();
			}
			if (lua.isTableNumber("right"))
			{
				right = lua.toNumber();
			}
			if (lua.isTableNumber("top"))
			{
				top = lua.toNumber();
			}
			if (lua.isTableNumber("bottom"))
			{
				bottom = lua.toNumber();
			}

			ScaleNineState scaleNineState = SCALE_NINE_NONE;
			bool vertical = top >= 0.0f && bottom >= 0.0f;
			bool horizontal = left >= 0.0f && right >= 0.0f;
			if (left < 0.0f || right < 0.0f)
			{
				left = 0.0f;
				right = 0.0f;
				if (mTexture)
				{
					right = static_cast<float>(mTexture->getWidth());
				}
			}
			if (top < 0.0f || bottom < 0.0f)
			{
				top = 0.0f;
				bottom = 0.0f;
				if (mTexture)
				{
					bottom = static_cast<float>(mTexture->getHeight());
				}
			}

			if (vertical && horizontal)
			{
				scaleNineState = SCALE_NINE_BOTH;
			}
			else if (vertical) 
			{
				scaleNineState = SCALE_NINE_VERTICAL;
			}
			else if (horizontal)
			{
				scaleNineState = SCALE_NINE_HORIZONTAL;
			}

			mScaleNineState = scaleNineState;
			mScaleNine.setInnerBounds(left, right, top, bottom);
			lua.pop(1);
		}

		if (lua.isTableBool("repeatX"))
		{
			mRepeatX = lua.toBool();
		}
		if (lua.isTableBool("repeatY"))
		{
			mRepeatY = lua.toBool();
		}

		{
			stringstream ss;
			ss << "Finish loading asset '" << mName << "'";
			am_log("ASSET", ss);
		}

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

		mWidth = mWindow.getWidth() / static_cast<float>(mNumFramesX);
		mHeight = mWindow.getHeight() / static_cast<float>(mNumFramesY);

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

	void Asset::assign(const Asset &rhs)
	{
		mName = rhs.mName;
		mTexture = rhs.mTexture;
		mWindow = rhs.mWindow;
		mNumFramesX = rhs.mNumFramesX;
		mNumFramesY = rhs.mNumFramesY;
		mTotalFrames = rhs.mTotalFrames;
		mFrameRate = rhs.mFrameRate;
		mWidth = rhs.mWidth;
		mHeight = rhs.mHeight;
		mAnimationDirty = rhs.mAnimationDirty;
		mScaleNine = rhs.mScaleNine;
		mScaleNineState = rhs.mScaleNineState;
		mAnimationWindows = rhs.mAnimationWindows;
		mRepeatX = rhs.mRepeatX;
		mRepeatY = rhs.mRepeatY;
	}

	void Asset::setRepeatX(bool repeat)
	{
		mRepeatX = repeat;
	}
	bool Asset::getRepeatX() const
	{
		return mRepeatX;
	}

	void Asset::setRepeatY(bool repeat)
	{
		mRepeatY = repeat;
	}
	bool Asset::getRepeatY() const
	{
		return mRepeatY;
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

	void Asset::setScaleNine(const ScaleNine &scaleNine)
	{
		mScaleNine = scaleNine;
	}
	void Asset::setScaleNine(const ScaleNine &scaleNine, ScaleNineState state)
	{
		mScaleNine = scaleNine;
		mScaleNineState = state;
	}
	ScaleNine &Asset::getScaleNine()
	{
		return mScaleNine;
	}

	void Asset::setScaleNineState(ScaleNineState state)
	{
		mScaleNineState = state;
	}
	ScaleNineState Asset::getScaleNineState() const
	{
		return mScaleNineState;
	}

}
}
