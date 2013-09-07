#pragma once

#include "event.h"

namespace am {
namespace gfx {
	class Renderable;
}
using namespace am::gfx;

namespace ui {

	class ListItem;

	class ListEvent : public Event {
	public:
		ListEvent(const char *type, ListItem *target, int index);
		~ListEvent();

		ListItem *getItem() const;
		int getIndex() const;

	protected:

		int mIndex;

	};

}
}
