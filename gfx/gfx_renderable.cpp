#include "gfx_renderable.h"

#include "gfx_engine.h"

namespace am {
namespace gfx {
	
	Renderable::Renderable(GfxEngine *engine) :
		GfxComponent(engine),
		mWidth(0),
		mHeight(0),
		mParent(NULL),
		mEnableInteractive(false),
		mVisible(true)
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
	float Renderable::getWidth() const
	{
		return mWidth;
	}

	void Renderable::setHeight(float height)
	{
		mHeight = height;
	}
	float Renderable::getHeight() const
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

	am::math::Transform &Renderable::getTransform()
	{
		return mTransform;
	}

}
}
