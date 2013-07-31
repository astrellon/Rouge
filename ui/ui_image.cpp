#include "ui_image.h"

#include "gfx/gfx_sprite.h"

namespace am {
namespace ui {

	Image::Image() :
		UIComponent(),
		mGraphic(nullptr)
	{
	}
	Image::Image(Asset *asset) :
		UIComponent()
	{
		mGraphic = new Sprite(asset);
		mWidth = mGraphic->getWidth();
		mHeight = mGraphic->getHeight();
		addChild(mGraphic.get());
	}
	Image::Image(const char *assetName) :
		UIComponent()
	{
		mGraphic = new Sprite(assetName);
		mWidth = mGraphic->getWidth();
		mHeight = mGraphic->getHeight();
		addChild(mGraphic.get());
	}
	Image::~Image()
	{
	}

	/*Colour &Image::getColour()
	{
		return mGraphic->getColour();
	}
	void Image::setColour(const Colour &colour)
	{
		mGraphic->setColour(colour);
	}
	void Image::setColour(float red, float green, float blue)
	{
		mGraphic->setColour(red, green, blue);
	}
	void Image::setColour(float red, float green, float blue, float alpha)
	{
		mGraphic->setColour(red, green, blue, alpha);
	}

	void Image::setAlpha(float alpha)
	{
		mGraphic->setAlpha(alpha);
	}
	float Image::getAlpha() const
	{
		return mGraphic->getAlpha();
	}*/

	void Image::setGfxComponent(GfxComponent *comp)
	{
		mGraphic->setGfxComponent(comp);
	}
	GfxComponent *Image::getGfxComponent() const
	{
		return mGraphic->getGfxComponent();
	}

	void Image::setAsset(Asset *asset)
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
	Sprite *Image::getGraphic()
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
