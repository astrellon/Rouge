#include "list_event.h"

#include <gfx/gfx_renderable.h>

#include <ui/ui_list.h>

namespace am {
namespace ui {

	ListEvent::ListEvent(const char *type, ListItem *target, int index) :
		Event(type, target),
		mIndex(index)
	{
	}
	ListEvent::~ListEvent()
	{
	}

	ListItem *ListEvent::getItem() const
	{
		return dynamic_cast<ListItem *>(mEventTarget.get());
	}

	int ListEvent::getIndex() const
	{
		return mIndex;
	}

}
}
