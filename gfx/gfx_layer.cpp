#include "gfx_layer.h"

#include "gfx_engine.h"

#include "../gl.h"

namespace am {
namespace gfx {

	Layer::Layer() :
		Renderable()
	{
		
	}
	Layer::Layer(const Layer &copy) :
		Renderable(copy)
	{
		for (auto iter = copy.mChildren.begin(); iter != copy.mChildren.end(); ++iter)
		{
			addChild((*iter)->clone());
		}
	}
	Layer::~Layer()
	{

	}

	Renderable *Layer::clone() const
	{
		return new Layer(*this);
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
			base::Handle<Layer> temp(this);
			mParent->removeChild(this);
		}
	}

	void Layer::clear()
	{
		ChildList::iterator iter;
		for (iter = mChildren.begin(); iter != mChildren.end(); ++iter)
		{
			(*iter)->setParent(nullptr);
		}
		mChildren.clear();
	}

	void Layer::addChild(Renderable *child)
	{
		if (child == nullptr)
		{
			return;
		}
		size_t i = findChild(child);
		if (i == -1u)
		{
			if (child->getParent() != nullptr)
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
		base::Handle<Renderable> childHandle(child);
		size_t i = findChild(child);
		if (child->getParent() != nullptr)
		{
			child->getParent()->removeChild(child);
		}

		if (i != -1u)
		{
			mChildren.erase(mChildren.begin() + i);
		}
		mChildren.insert(mChildren.begin() + index, child);
		child->setParent(this);
	}
	void Layer::removeChild(Renderable *child)
	{
		if (child == nullptr)
		{
			return;
		}
		size_t i = findChild(child);
		if (i != -1u)
		{
			child->setParent(nullptr);
			mChildren.erase(mChildren.begin() + i);
		}
	}
	bool Layer::hasChild(Renderable *child) const
	{
		if (child == nullptr)
		{
			return false;
		}
		return findChild(child) != -1u;
	}
	Renderable *Layer::getChildAt(int index) const
	{
		if (index >= static_cast<int>(mChildren.size()) ||
			index < 0)
		{
			return nullptr;
		}
		return mChildren[index];
	}

	int Layer::getNumChildren() const
	{
		return static_cast<int>(mChildren.size());
	}

	bool Layer::interactWithLayer() const
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
			if (iter->get())
			{
				(*iter)->render(dt);
			}
		}

		postRender(dt);
	}
	
	size_t Layer::findChild(Renderable *child) const
	{
        for (size_t i = 0; i < mChildren.size(); i++)
		{
			if (mChildren[i].get() == child)
			{
				return i;
			}
		}
		return -1u;
	}

	std::string Layer::getName() const
	{
		std::stringstream name;
#ifdef _DEBUG
		name << mDebugName;
		if (mDebugName.size() == 0)
		{
			name << "Layer";
		}
#else
		name << "Layer";
#endif
		name << " (" << getNumChildren() << ")";
		return name.str();
	}

}
}
