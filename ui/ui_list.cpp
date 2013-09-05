#include "ui_list.h"

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_engine.h>
#include <gfx/gfx_texture.h>

#include <gl.h>

namespace am {
namespace ui {

	List::List() :
		UIComponent(),
		mItemDisplay(0),
		mScroll(0),
		mScrollbar(new Scrollbar("ui:scrollbar_up", "ui:scrollbar_down", "ui:scrollbar_bar", "ui:scrollbar_back"))
	{
		addChild(mScrollbar);
		mScrollbar->addEventListener(Scrollbar::SCROLL_VALUE_CHANGE, this);
		
	}
	List::~List()
	{

	}

	void List::onEvent(Event *e)
	{
		if (e->getEventTarget() == mScrollbar.get())
		{
			setScroll(mScrollbar->getValue());
		}
	}

	void List::setWidth(float width)
	{
		if (width != mWidth)
		{
			//mDirty = true;
			Renderable::setWidth(width);
			mScrollbar->setPosition(width - mScrollbar->getWidth(), 0.0f);
			for (auto iter = mItems.begin(); iter != mItems.end(); ++iter)
			{
				iter->get()->setWidth(width - 8.0f);
			}
		}
	}
	void List::setHeight(float height)
	{
		if (height != mHeight)
		{
			//mDirty = true;
			Renderable::setHeight(height);
			mScrollbar->setHeight(height);
		}
	}

	bool List::addItem(Renderable *item, int index)
	{
		if (index < 0)
		{
			if (hasItem(item))
			{
				return false;
			}
			mItems.push_back(item);
			updateScrollbar();
			item->setWidth(getWidth() - 8.0f);
			return true;
		}
		Handle<Renderable> itemHandle(item);
		int i = findItem(item);
		if (i >= 0)
		{
			mItems.erase(mItems.begin() + i);
		}
		mItems.insert(mItems.begin() + index, item);
		item->setWidth(getWidth() - 8.0f);
		updateScrollbar();
		return true;
	}
	bool List::removeItem(Renderable *item)
	{
		int index = findItem(item);
		if (index < 0)
		{
			return false;
		}
		mItems.erase(mItems.begin() + index);
		updateScrollbar();
		return true;
	}
	bool List::removeItem(int index)
	{
		if (index < 0 || index >= static_cast<int>(mItems.size()))
		{
			return false;
		}
		mItems.erase(mItems.begin() + index);
		updateScrollbar();
		return true;
	}
	bool List::hasItem(Renderable *item) const
	{
		return findItem(item) >= 0;
	}
	Renderable *List::getItem(int index) const
	{
		if (index < 0 || index >= static_cast<int>(mItems.size()))
		{
			return nullptr;
		}
		return mItems[index];
	}
	const List::ListItems &List::getItems() const
	{
		return mItems;
	}

	void List::setItemDisplay(int display)
	{
		mItemDisplay = display;
	}
	int List::getItemDisplay() const
	{
		return mItemDisplay;
	}

	void List::setScroll(int scroll)
	{
		mScroll = scroll;
	}
	int List::getScroll() const
	{
		return mScroll;
	}

	int List::findItem(Renderable *item) const
	{
		for (size_t i = 0; i < mItems.size(); ++i)
		{
			if (mItems[i].get() == item)
			{
				return static_cast<int>(i);
			}
		}
		return -1;
	}

	void List::preRender(float dt)
	{
		UIComponent::preRender(dt);

		glColor4f(0.1f, 0.1f, 0.1f, 0.4f);
		//glColor3d(1, 1, 1);
		Texture::bindTexture(0);
		glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(getWidth(), 0.0f);
		glVertex2f(getWidth(), getHeight());
		glVertex2f(0.0f, getHeight());
		glEnd();
		GfxEngine::getEngine()->applyColourStack();
	}

	void List::postRender(float dt)
	{
		glPushMatrix();
		glTranslatef(4.0f, 4.0f, 0.0f);
		int display = 0;
		for (int index = max(0, mScroll); index < mItems.size(); ++index)
		{
			if (mItemDisplay > 0 && display >= mItemDisplay)
			{
				break;
			}
			display++;
			Renderable *item = mItems[index].get();
			if (!item->isVisible())
			{
				continue;
			}
			item->render(dt);
			glTranslatef(0.0f, item->getHeight() + 4.0f, 0.0f);
		}
		glPopMatrix();

		UIComponent::postRender(dt);
	}

	void List::updateScrollbar()
	{
		int maximum = max(0, mItems.size() - mItemDisplay);
		mScrollbar->setMaxValue(maximum);
	}

}
}
