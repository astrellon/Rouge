#pragma once

#include <base/handle.h>
using namespace am::base;

#include <string>
#include <vector>
using namespace std;

#include <gfx/gfx_renderable.h>
#include <gfx/gfx_scrollbar.h>
using namespace am::gfx;

#include "ui_component.h"

namespace am {
namespace ui {

	class List : public UIComponent, public IEventListener {
	public:

		typedef vector< Handle<Renderable> > ListItems;

		List();
		~List();

		virtual bool addItem(Renderable *item, int index = -1);
		virtual bool removeItem(Renderable *item);
		virtual bool removeItem(int index);
		virtual bool hasItem(Renderable *item) const;
		virtual Renderable *getItem(int index) const;
		virtual const ListItems &getItems() const;

		virtual void setItemDisplay(int display);
		virtual int getItemDisplay() const;

		virtual void setScroll(int scroll);
		virtual int getScroll() const;

		virtual void onEvent(Event *e);

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		//virtual float getMeasuredWidth();
		//virtual float getMeasuredHeight();

	protected:

		ListItems mItems;
		int mItemDisplay;
		int mScroll;
		Handle<Scrollbar> mScrollbar;

		int findItem(Renderable *item) const;

		virtual void preRender(float dt);
		virtual void postRender(float dt);
		virtual void updateScrollbar();

	};

}
}
