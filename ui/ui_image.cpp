#include "ui_image.h"

#include <gfx/gfx_sprite.h>

namespace am {
namespace ui {

	Image::Image() :
		UIComponent(),
		mGraphic(nullptr)
	{
	}
	Image::Image(gfx::Asset *asset) :
		UIComponent()
	{
		mGraphic = new gfx::Sprite(asset);
		mWidth = mGraphic->getWidth();
		mHeight = mGraphic->getHeight();
		addChild(mGraphic.get());
	}
	Image::Image(const char *assetName) :
		UIComponent()
	{
		mGraphic = new gfx::Sprite(assetName);
		mWidth = mGraphic->getWidth();
		mHeight = mGraphic->getHeight();
		addChild(mGraphic.get());
	}
	Image::~Image()
	{
	}

	void Image::setGfxComponent(gfx::GfxComponent *comp)
	{
		mGraphic->setGfxComponent(comp);
	}
	gfx::GfxComponent *Image::getGfxComponent() const
	{
		return mGraphic->getGfxComponent();
	}

	void Image::setAsset(gfx::Asset *asset)
	{
		mGraphic->setAsset(asset);
		mWidth = mGraphic->getWidth();
		mHeight = mGraphic->getHeight();
	}
	void Image::setAsset(const char *assetName)
	{
		mGraphic->setAsset(assetName);
		mWidth = mGraphic->getWidth();
		mHeight = mGraphic->getHeight();
	}
	gfx::Sprite *Image::getGraphic()
	{
		return mGraphic.get();
	}

	void Image::setWidth(float width)
	{
		UIComponent::setWidth(width);
		mGraphic->setWidth(mWidth);
	}
	void Image::setHeight(float height)
	{
		UIComponent::setHeight(height);
		mGraphic->setHeight(mHeight);
	}

}
}
