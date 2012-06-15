#include "ui_image.h"

#include "gfx/gfx_sprite.h"

namespace am {
namespace ui {

	Image::Image() :
		UIComponent(),
		mGraphic(NULL)
	{
	}
	Image::Image(Asset *asset) :
		UIComponent()
	{
		mGraphic = new Sprite(asset);
		addChild(mGraphic.get());
	}
	Image::Image(const char *assetName) :
		UIComponent()
	{
		mGraphic = new Sprite(assetName);
		addChild(mGraphic.get());
	}
	Image::~Image()
	{
	}

	Colour &Image::getColour()
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

	void Image::setWidth(float width)
	{
		mGraphic->setWidth(width);
		UIComponent::setWidth(width);
	}
	void Image::setHeight(float height)
	{
		mGraphic->setHeight(height);
		UIComponent::setHeight(height);
	}

}
}
