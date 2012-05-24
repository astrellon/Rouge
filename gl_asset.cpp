#include "gl_asset.h"

#include "gl_texture.h"
#include "gl_gfx_engine.h"

namespace am {
namespace gfx {

	// GlAsset methods
	GlAsset::GlAsset(GlGfxEngine *engine, const char *name) :
		mGfxEngine(engine),
		mTexture(NULL),
		mName(name)
	{

	}
	GlAsset::~GlAsset()
	{

	}

	void GlAsset::setGlTexture(const GlTexture *texture)
	{
		if (texture != NULL)
		{
			mTexture = texture;
		}
	}
	const GlTexture *GlAsset::getGlTexture() const
	{
		return mTexture;
	}

	// IGfxAsset methods
	const char *GlAsset::getName() const
	{
		return mName.c_str();
	}

	void GlAsset::setTexture(const ITexture *texture)
	{
		setGlTexture(dynamic_cast<const GlTexture *>(texture));
	}
	const ITexture *GlAsset::getTexture() const
	{
		return mTexture;
	}

	void GlAsset::setTextureWindow(const TextureWindow &window)
	{
		mWindow = window;
	}
	const TextureWindow &GlAsset::getTextureWindow() const
	{
		return mWindow;
	}

	int GlAsset::loadDef(JsonValue value)
	{
		if (value.has("texture", JV_STR))
		{
			setGlTexture(mGfxEngine->getGlTexture(value["texture"].getCStr()));
		}
		else
		{
			return -1;
		}
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

	// IGfxComponent methods
	IGfxEngine *GlAsset::getGfxEngine()
	{
		return mGfxEngine;
	}
	// IGlGfxComponent methods
	GlGfxEngine *GlAsset::getGlGfxEngine()
	{
		return mGfxEngine;
	}
	
}
}
