#pragma once

#include <base/handle.h>

#include <string>
#include <vector>
using namespace std;

#include <gfx/gfx_renderable.h>
#include <gfx/gfx_scrollbar.h>
using namespace am::gfx;

#include "ui_component.h"

namespace am {
namespace ui {

	class ListItem : public Layer, public IEventListener {
	public:
		ListItem();
		~ListItem();

		void setIndex(int index);
		int getIndex() const;

	protected:
		int mIndex;
	};

	class List : public UIComponent, public IEventListener {
	public:

		typedef vector< am::base::Handle<ListItem> > ListItems;

		List();
		~List();

		virtual bool addItem(ListItem *item, int index = -1);
		virtual bool removeItem(ListItem *item);
		virtual bool removeItem(int index);
		virtual bool hasItem(ListItem *item) const;
		virtual ListItem *getItem(int index) const;
		virtual const ListItems &getItems() const;

		virtual void setItemDisplay(int display);
		virtual int getItemDisplay() const;

		virtual void setScroll(int scroll);
		virtual int getScroll() const;

		virtual void onEvent(Event *e);

		virtual void setWidth(float width);
		virtual void setHeight(float height);

	protected:

		ListItems mItems;
		int mItemDisplay;
		int mScroll;
		am::base::Handle<Scrollbar> mScrollbar;
		am::base::Handle<Layer> mListRenderer;

		int findItem(ListItem *item) const;

		virtual void preRender(float dt);
		virtual void updateScrollbar();

		virtual void _addItem(ListItem *item);
		virtual void updateList();

	};

}
}
