#include "ui_list.h"

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_engine.h>
#include <gfx/gfx_texture.h>

#include <gl.h>

#include <algorithm>

namespace am {
namespace ui {

	ListItem::ListItem() :
		Layer(),
		mIndex(-1)
	{

	}
	ListItem::~ListItem()
	{

	}

	void ListItem::setIndex(int index)
	{
		mIndex = index;
	}
	int ListItem::getIndex() const
	{
		return mIndex;
	}

	List::List() :
		UIComponent(),
		mItemDisplay(0),
		mScroll(0),
		mScrollbar(new gfx::Scrollbar("ui:scrollbar_up", "ui:scrollbar_down", "ui:scrollbar_bar", "ui:scrollbar_back")),
		mListRenderer(new gfx::Layer())
	{
		addChild(mScrollbar);
		mScrollbar->addEventListener(gfx::Scrollbar::SCROLL_VALUE_CHANGE, this);
		addChild(mListRenderer);
		mListRenderer->setInteractive(true);
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
		else if (e->getType() == "click")
		{
			int index = 0;
			for (size_t i = 0; i < mItems.size(); i++)
			{
				if (mItems[i].get() == e->getEventTarget())
				{
					index = static_cast<int>(i);
					break;
				}
			}
			base::Handle<ListEvent> e(new ListEvent("list_change", mItems[index], index));
			fireEvent<ListEvent>(e);
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
				iter->get()->setWidth(width - 8.0f - mScrollbar->getWidth());
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

	bool List::addItem(ListItem *item, int index)
	{
		if (index < 0)
		{
			if (hasItem(item))
			{
				return false;
			}
			mItems.push_back(item);
			_addItem(item);
			return true;
		}
		base::Handle<Renderable> itemHandle(item);
		int i = findItem(item);
		if (i >= 0)
		{
			mItems.erase(mItems.begin() + i);
		}
		mItems.insert(mItems.begin() + index, item);
		_addItem(item);
		return true;
	}
	void List::_addItem(ListItem *item)
	{
		updateScrollbar();
		item->setWidth(getWidth() - 8.0f - mScrollbar->getWidth());
		item->addEventListener("click", this);
		mListRenderer->addChild(item);
		updateList();
	}
	bool List::removeItem(ListItem *item)
	{
		int index = findItem(item);
		if (index < 0)
		{
			return false;
		}
		item->removeEventListener("click", this);
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
		mItems[index]->removeEventListener("click", this);
		mItems.erase(mItems.begin() + index);
		mListRenderer->removeChild(mItems[index]);
		updateScrollbar();
		return true;
	}
	bool List::hasItem(ListItem *item) const
	{
		return findItem(item) >= 0;
	}
	ListItem *List::getItem(int index) const
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
		for (size_t i = 0; i < mItems.size(); i++)
		{
			mItems[i]->setVisible(i >= scroll);
		}
		updateList();
	}
	int List::getScroll() const
	{
		return mScroll;
	}

	int List::findItem(ListItem *item) const
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
		gfx::Texture::bindTexture(0);
		glBegin(GL_QUADS);
		glVertex2f(0.0f, 0.0f);
		glVertex2f(getWidth(), 0.0f);
		glVertex2f(getWidth(), getHeight());
		glVertex2f(0.0f, getHeight());
		glEnd();
		gfx::GfxEngine::getEngine()->applyColourStack();
	}

	void List::updateScrollbar()
	{
		int maximum = std::max(0, static_cast<int>(mItems.size() - mItemDisplay));
		mScrollbar->setMaxValue(maximum);
	}

	void List::updateList()
	{
		float y = 4.0f;
		int displayed = 0;
		int scrollTo = mScroll;
		int index = 0;
		for (auto iter = mItems.begin(); iter != mItems.end(); ++iter, ++index)
		{
			ListItem *item = iter->get();
			item->setIndex(index);
			if (scrollTo < 0)
			{
				scrollTo++;
				item->setVisible(false);
				continue;
			}
			if (!item->isVisible())
			{
				continue;
			}
			item->setPosition(4.0f, y);
			y += item->getHeight() + 4.0f;
			displayed++;
			if (mItemDisplay > 0 && displayed >= mItemDisplay)
			{
				break;
			}
		}
	}



}
}
