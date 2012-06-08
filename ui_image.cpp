#include "ui_image.h"

#include "gfx/gfx_engine.h"
#include "gfx/gfx_sprite.h"

namespace am {
namespace ui {

	Image::Image(GfxEngine *engine) :
		UIComponent(engine),
		mGraphic(NULL)
	{
	}
	Image::Image(GfxEngine *engine, Asset *asset) :
		UIComponent(engine)
	{
		mGraphic = new Sprite(engine, asset);
		addChild(mGraphic.get());
	}
	Image::Image(GfxEngine *engine, const char *assetName) :
		UIComponent(engine)
	{
		mGraphic = new Sprite(engine, assetName);
		addChild(mGraphic.get());
	}
	Image::~Image()
	{
	}

	void Image::setAsset(Asset *asset)
	{
		mGraphic->setAsset(asset);
	}
	void Image::setAsset(const char *assetName)
	{
		mGraphic->setAsset(assetName);
	}
	Sprite *Image::getGraphic()
	{
		return mGraphic.get();
	}

}
}
