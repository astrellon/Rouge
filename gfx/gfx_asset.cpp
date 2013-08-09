#include "gfx_asset.h"

#include "gfx_texture.h"
#include "gfx_engine.h"

#include <log/logger.h>

#include <util/utils.h>
using namespace am::util;

#include <string>

using namespace std;

namespace am {
namespace gfx {

	// Asset methods
	Asset::Asset(const char *name) :
		IManaged(),
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

	void Asset::setTexture(const Texture *texture, unsigned int index)
	{
		if (texture != nullptr && texture->isLoaded())
		{
			// If we're adding more textures then we need to make sure that they
			// are all the same size.
			if (mTextures.size() > 0u)
			{
				if (mTextures[0]->getWidth() != texture->getWidth() ||
					mTextures[0]->getHeight() != texture->getHeight())
				{
					return;
				}
			}
			if (index >= mTextures.size())
			{
				mTextures.resize(index + 1);
			}
			mTextures[index] = texture;

			if (mTextures.size() == 1u)
			{
				mWindow.setValues(static_cast<float>(texture->getWidth()), 
					static_cast<float>(texture->getHeight()),
					0.0f, 1.0f, 0.0f, 1.0f);
			}
		}
	}
	const Texture *Asset::getTexture(unsigned int index) const
	{
		if (index >= mTextures.size())
		{
			return nullptr;
		}
		return mTextures[index];
	}

	void Asset::addTexture(const Texture *texture)
	{
		setTexture(texture, mTextures.size());
	}
	void Asset::removeTexture(const Texture *texture)
	{
		int index = Utils::find(mTextures, texture);
		if (index == -1)
		{
			return;
		}
		mTextures.erase(mTextures.begin() + index);
	}
	bool Asset::hasTexture(const Texture *texture) const
	{
		return Utils::find(mTextures, texture) != -1;
	}
	int Asset::getNumberOfTextures() const
	{
		return static_cast<int>(mTextures.size());
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
		if (lua.isTableTable("textures"))
		{
			lua_pushnil(lua);
			while (lua_next(lua, -2) != 0) 
			{
				/* uses 'key' (at index -2) and 'value' (at index -1) */
				if (lua_isstr(lua, -1))
				{
					const char *textureName = lua_tostring(lua, -1);
					Texture *texture = GfxEngine::getEngine()->getTexture(textureName);
					if (!texture)
					{
						stringstream errss;
						errss << "Unable to load texture ("<< textureName << ") for asset '"
							<< mName.c_str() << '\'';
						am_log("ASSET", errss);
						lua_pop(lua, 1);
						return -1;
					}
					addTexture(texture);
					lua_pop(lua, 1);
				}
			}
			lua_pop(lua, 1);
		}
		if (mTextures.size() == 0)
		{
			if (lua.isTableString("texture"))
			{
				const char *textureName = lua_tostring(lua, -1);
				lua.pop(1);
				Texture *texture = GfxEngine::getEngine()->getTexture(textureName);
				if (!texture)
				{
					stringstream errss;
					errss << "Unable to load texture ("<< textureName << ") for asset '"
						<< mName.c_str() << '\'';
					am_log("ASSET", errss);
					return -1;
				}
				setTexture(texture);

				/*const char *currentTexture = nullptr;
				if (mTextures.size() != nullptr)
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
					if (mTexture == nullptr)
					{
						stringstream errss;
						errss << "Unable to load texture ("<< texture << ") for asset '"
							<< mName.c_str() << '\'';
						am_log("ASSET", errss);
						return -1;
					}
				}*/
			}
			else
			{
				return -1;
			}
		}
		/*{
			stringstream ss;
			ss << "Loading asset '" << mName.c_str() << '\'';
			am_log("ASSET", ss);
		}*/
		float textureWidth = static_cast<float>(mTextures[0]->getWidth());
		float textureHeight = static_cast<float>(mTextures[0]->getHeight());
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
			
			if (lua.isTableNumber("left"))
			{
				winLeftX = lua.toNumber();
				leftX = winLeftX / textureWidth;
			}
			if (lua.isTableNumber("right"))
			{
				float winRightX = lua.toNumber();
				width = winRightX - winLeftX;
				rightX = winRightX / textureWidth;
			}

			if (lua.isTableNumber("top"))
			{
				winTopY = lua.toNumber();
				topY = winTopY / textureHeight;
			}

			if (lua.isTableNumber("bottom"))
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
		//LuaState::logStack(lua, mName.c_str());
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
				right = static_cast<float>(mTextures[0]->getWidth());
			}
			if (top < 0.0f || bottom < 0.0f)
			{
				top = 0.0f;
				bottom = static_cast<float>(mTextures[0]->getHeight());
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

		/*{
			stringstream ss;
			ss << "Finish loading asset '" << mName << "'";
			am_log("ASSET", ss);
		}*/

		return 0;
	}

	void Asset::processAnimation()
	{
		if (!mAnimationDirty || mTextures.size() == 0)
		{
			return;
		}

		float textureWidth = static_cast<float>(mTextures[0]->getWidth());
		float textureHeight = static_cast<float>(mTextures[0]->getHeight());

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
		mTextures = rhs.mTextures;
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
