#pragma once

#include <string>

#include "event.h"

namespace am {
namespace game {
	class Item;
	class GameObject;
}
using namespace am::game;

namespace ui {

	class ItemEvent : public Event {
	public:
		ItemEvent(const char *type, Item *item, GameObject *extra = nullptr);
		~ItemEvent();

		Item *getItem() const;
		GameObject *getExtra() const;

	protected:

		Item *mItem;
		GameObject *mExtra;
	};

}
}
