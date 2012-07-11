#include "gfx_renderable.h"

#include <math/vector.h>
using namespace am::math;

#include <base/handle.h>
using namespace am::base;

#include <gl.h>
#include "gfx_engine.h"
#include "gfx_layer.h"

namespace am {
namespace gfx {

	std::ostream& operator<<(std::ostream &strm, const Renderable &rhs)
	{
		strm << "Renderable(" << rhs.getName() << ")";
		return strm;
	}
	
	Renderable::Renderable() :
		IManaged(),
		mWidth(0),
		mHeight(0),
		mParent(NULL),
		mInteractive(false),
		mVisible(true),
		mColour(1.0f, 1.0f, 1.0f, 1.0f)
	{
		
	}
	Renderable::~Renderable()
	{
		if (mParent)
		{
			mParent->release();
		}
	}

	void Renderable::preRender(float dt)
	{
		glPushMatrix();
		//glMultMatrixf(mTransform.data());
		mTransform.apply();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GfxEngine::getEngine()->pushColourStack(mColour);
		GfxEngine::getEngine()->applyColourStack();
		//mColour.applyColour();
	}
	void Renderable::render(float dt)
	{
	}
	void Renderable::postRender(float dt)
	{
		glPopMatrix();
		GfxEngine::getEngine()->popColourStack();

		glDisable(GL_BLEND);
	}

	void Renderable::setSize(float width, float height)
	{
		setWidth(width);
		setHeight(height);
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
		if (mParent)
		{
			mParent->release();
		}
		mParent = layer;
		if (mParent)
		{
			mParent->retain();
		}
	}

	void Renderable::setInteractive(bool set)
	{
		mInteractive = set;
	}
	bool Renderable::isInteractive() const
	{
		return mInteractive;
	}

	void Renderable::setVisible(bool visible)
	{
		mVisible = visible;
	}
	bool Renderable::isVisible() const
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

	void Renderable::setAlpha(float alpha)
	{
		mColour.setAlpha(alpha);
	}
	float Renderable::getAlpha() const
	{
		return mColour.getAlpha();
	}

	am::math::TransformLite &Renderable::getTransform()
	{
		return mTransform;
	}
	void Renderable::setPosition(float x, float y)
	{
		mTransform.setXY(x, y);
	}
	void Renderable::setPosition(int x, int y)
	{
		mTransform.setXY(static_cast<float>(x), static_cast<float>(y));
	}
	float Renderable::getPositionX() const
	{
		return mTransform.getX();
	}
	float Renderable::getPositionY() const
	{
		return mTransform.getY();
	}

	void Renderable::setName(const char *name)
	{
#ifdef _DEBUG
		mDebugName = name;
#endif
	}
	string Renderable::getName() const
	{
#ifdef _DEBUG
		return mDebugName;
#else
		return "Renderable";
#endif
	}

}
}
