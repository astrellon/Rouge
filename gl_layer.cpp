#include "gl_layer.h"

#include "gl_gfx_engine.h"
#include "igl_renderable.h"

#include "gl.h"

namespace am {
namespace gfx {

	GlLayer::GlLayer(GlGfxEngine *engine) :
		mWidth(0),
		mHeight(0),
		mGfxEngine(engine)
	{
		mTransform.setUpDirection(am::math::Transform::REF_FORWARD);
	}
	GlLayer::~GlLayer()
	{

	}

	void GlLayer::addGlChild(IGlRenderable *child)
	{
		if (child == NULL)
		{
			return;
		}
		ChildList::const_iterator iter = findChild(child);
		if (iter == mChildren.end())
		{
			mChildren.push_back(child);
		}
	}
	void GlLayer::removeGlChild(IGlRenderable *child)
	{
		if (child == NULL)
		{
			return;
		}
		ChildList::const_iterator iter = findChild(child);
		if (iter != mChildren.end())
		{
			mChildren.erase(iter);
		}
	}
	bool GlLayer::hasGlChild(IGlRenderable *child) const
	{
		if (child == NULL)
		{
			return false;
		}
		return findChild(child) != mChildren.end();
	}
	IGlRenderable *GlLayer::getGlChildAt(int index) const
	{
		if (index >= static_cast<int>(mChildren.size()) ||
			index < 0)
		{
			return NULL;
		}
		return mChildren[index];
	}

	// ILayer methods
	void GlLayer::addChild(IRenderable *child)
	{
		addGlChild(dynamic_cast<IGlRenderable *>(child));
	}
	void GlLayer::removeChild(int index)
	{
		//mChildren.erase(index);
		mChildren.erase(mChildren.begin() + index);
	}
	void GlLayer::removeChild(IRenderable *child)
	{
		removeGlChild(dynamic_cast<IGlRenderable *>(child));
	}

	bool GlLayer::hasChild(IRenderable *child) const
	{
		return hasGlChild(dynamic_cast<IGlRenderable *>(child));
	}
	int GlLayer::getNumChildren() const
	{
		return static_cast<int>(mChildren.size());
	}
	IRenderable *GlLayer::getChildAt(int index) const
	{
		return getGlChildAt(index);
	}

	// IRenderable methods
	void GlLayer::render(float dt)
	{
		glPushMatrix();
		glMultMatrixf(mTransform.data());

		ChildList::iterator iter;
		for (iter = mChildren.begin(); iter != mChildren.end(); ++iter)
		{
			(*iter)->render(dt);
		}

		glPopMatrix();
	}
	am::math::Transform &GlLayer::getTransform()
	{
		return mTransform;
	}

	void GlLayer::setWidth(float width)
	{
		mWidth = width;
	}
	float GlLayer::getWidth() const
	{
		return mWidth;
	}

	void GlLayer::setHeight(float height)
	{
		mHeight = height;
	}
	float GlLayer::getHeight() const
	{
		return mHeight;
	}

	// IGfxComponent methods
	IGfxEngine *GlLayer::getGfxEngine()
	{
		return mGfxEngine;
	}
	// IGlGfxComponent methods
	GlGfxEngine *GlLayer::getGlGfxEngine()
	{
		return mGfxEngine;
	}

	GlLayer::ChildList::const_iterator GlLayer::findChild(IGlRenderable *child) const
	{
		ChildList::const_iterator iter;
		for (iter = mChildren.begin(); iter != mChildren.end(); ++iter)
		{
			if (*iter == child)
			{
				return iter;
			}
		}
		return mChildren.end();
	}

}
}
