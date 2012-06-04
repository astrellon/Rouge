#include "gfx_renderable.h"

#include "../vector.h"

#include "gfx_engine.h"

namespace am {
namespace gfx {
	
	Renderable::Renderable(GfxEngine *engine) :
		GfxComponent(engine),
		mWidth(0),
		mHeight(0),
		mParent(NULL),
		mEnableInteractive(false),
		mVisible(true),
		mColour(1.0f, 1.0f, 1.0f, 1.0f)
	{
		mTransform.setUpDirection(am::math::Transform::REF_FORWARD);
	}
	Renderable::~Renderable()
	{
	}

	void Renderable::render(float dt)
	{
	}

	void Renderable::setWidth(float width)
	{
		mWidth = width;
	}
	float Renderable::getWidth()
	{
		return mWidth;
	}

	void Renderable::setHeight(float height)
	{
		mHeight = height;
	}
	float Renderable::getHeight()
	{
		return mHeight;
	}

	Layer *Renderable::getParent() const
	{
		return mParent;
	}
	void Renderable::setParent(Layer *layer)
	{
		mParent = layer;
	}

	void Renderable::setEnableInteractive(bool set)
	{
		mEnableInteractive = set;
	}
	bool Renderable::getEnableInteractive() const
	{
		return mEnableInteractive;
	}

	void Renderable::setVisible(bool visible)
	{
		mVisible = visible;
	}
	bool Renderable::getVisible() const
	{
		return mVisible;
	}

	Colour &Renderable::getColour()
	{
		return mColour;
	}
	void Renderable::setColour(const Colour &colour)
	{
		mColour = colour;
	}
	void Renderable::setColour(float red, float green, float blue)
	{
		mColour.setColour(red, green, blue);
	}
	void Renderable::setColour(float red, float green, float blue, float alpha)
	{
		mColour.setColour(red, green, blue, alpha);
	}

	am::math::Transform &Renderable::getTransform()
	{
		return mTransform;
	}
	void Renderable::setPosition(float x, float y)
	{
		mTransform.setPosition(Vector4f(x, y, 0.0f));
	}
	void Renderable::setPosition(int x, int y)
	{
		mTransform.setPosition(Vector4f(static_cast<float>(x), static_cast<float>(y), 0.0f));
	}

}
}