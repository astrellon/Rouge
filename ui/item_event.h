#pragma once

#include <string>

#include "event.h"

namespace am {
namespace game {
	class Item;
	class GameObject;
}

namespace ui {

	class ItemEvent : public Event
	{
	public:
		ItemEvent(const char *type, game::Item *item, game::GameObject *extra = nullptr);
		~ItemEvent();

		game::Item *getItem() const;
		game::GameObject *getExtra() const;

	protected:

		game::Item *mItem;
		game::GameObject *mExtra;
	};

}
}
