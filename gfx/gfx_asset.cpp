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
		mName(name)
	{

	}
	Asset::~Asset()
	{

	}

	void Asset::setTexture(const Texture *texture)
	{
		if (texture != NULL)
		{
			mTexture = texture;
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

		return 0;
	}

}
}
