#include "gfx_layer.h"

#include "gfx_engine.h"

#include "../gl.h"

namespace am {
namespace gfx {

	Layer::Layer() :
		Renderable()
	{
		
	}
	Layer::~Layer()
	{

	}

	void Layer::deinit()
	{
		ChildList children = mChildren;
		for (size_t i = 0; i < children.size(); i++)
		{
			children[i]->deinit();
		}

		/*while(mChildren.size())
		{
			mChildren.back()->deinit();
		}*/
		children.clear();
		mChildren.clear();
		if (mParent)
		{
			Handle<Layer> temp(this);
			mParent->removeChild(this);
		}
	}

	void Layer::clear()
	{
		ChildList::iterator iter;
		for (iter = mChildren.begin(); iter != mChildren.end(); ++iter)
		{
			(*iter)->setParent(NULL);
		}
		mChildren.clear();
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
				if (child->getParent() == this)
				{
					return;
				}
				child->getParent()->removeChild(child);
			}
			mChildren.push_back(child);
			child->setParent(this);
		}
	}
	void Layer::addChild(Renderable *child, int index)
	{
		if (index >= static_cast<int>(mChildren.size()))
		{
			addChild(child);
			return;
		}

		// Since we are removing the child before adding it in again
		// if it is found. We don't want it being deleted if it only has
		// one reference to it.
		Handle<Renderable> childHandle(child);
		ChildList::const_iterator iter = findChild(child);
		if (child->getParent() != NULL)
		{
			child->getParent()->removeChild(child);
		}

		if (iter != mChildren.end())
		{
			mChildren.erase(iter);
		}
		mChildren.insert(mChildren.begin() + index, child);
		child->setParent(this);
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
			child->setParent(NULL);
			mChildren.erase(iter);
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

	bool Layer::interacteWithLayer() const
	{
		return false;
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
			if (mGfxComponent)
			{
				mGfxComponent->getColour().applyColour();
			}
			(*iter)->render(dt);
		}

		postRender(dt);
	}
	
	Layer::ChildList::const_iterator Layer::findChild(Renderable *child) const
	{
		ChildList::const_iterator iter;
		for (iter = mChildren.begin(); iter != mChildren.end(); ++iter)
		{
			if (iter->get() == child)
			{
				return iter;
			}
		}
		return mChildren.end();
	}

}
}
