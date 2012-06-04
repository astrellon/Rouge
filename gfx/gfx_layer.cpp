#include "gfx_layer.h"

#include "gfx_engine.h"

#include "../gl.h"

namespace am {
namespace gfx {

	Layer::Layer(GfxEngine *engine) :
		Renderable(engine)
	{
		
	}
	Layer::~Layer()
	{

	}

	void Layer::addChild(Renderable *child)
	{
		if (child == NULL)
		{
			return;
		}
		ChildList::const_iterator iter = findChild(child);
		if (iter == mChildren.end())
		{
			if (child->getParent() != NULL)
			{
				child->getParent()->removeChild(child);
			}
			mChildren.push_back(child);
			child->setParent(this);
		}
	}
	void Layer::removeChild(Renderable *child)
	{
		if (child == NULL)
		{
			return;
		}
		ChildList::const_iterator iter = findChild(child);
		if (iter != mChildren.end())
		{
			mChildren.erase(iter);
			child->setParent(NULL);
		}
	}
	bool Layer::hasChild(Renderable *child) const
	{
		if (child == NULL)
		{
			return false;
		}
		return findChild(child) != mChildren.end();
	}
	Renderable *Layer::getChildAt(int index) const
	{
		if (index >= static_cast<int>(mChildren.size()) ||
			index < 0)
		{
			return NULL;
		}
		return mChildren[index];
	}

	int Layer::getNumChildren() const
	{
		return static_cast<int>(mChildren.size());
	}

	// IRenderable methods
	void Layer::render(float dt)
	{
		if (!mVisible)
		{
			return;
		}
		preRender(dt);

		ChildList::iterator iter;
		for (iter = mChildren.begin(); iter != mChildren.end(); ++iter)
		{
			mColour.applyColour();
			(*iter)->render(dt);
		}

		postRender(dt);
	}
	
	Layer::ChildList::const_iterator Layer::findChild(Renderable *child) const
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
