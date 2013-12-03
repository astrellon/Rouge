#include "item_event.h"

#include <game/item.h>
#include <game/game_object.h>

namespace am {
namespace ui {

	ItemEvent::ItemEvent(const char *type, game::Item *item, game::GameObject *extra) :
		Event(type),
		mItem(item),
		mExtra(extra)
	{
		if (item)
		{
			item->retain();
		}
		if (extra)
		{
			extra->retain();
		}
	}
	ItemEvent::~ItemEvent()
	{
		if (mItem)
		{
			mItem->release();
		}
		if (mExtra)
		{
			mExtra->release();
		}
	}
	
	game::Item *ItemEvent::getItem() const
	{
		return mItem;
	}
	game::GameObject *ItemEvent::getExtra() const
	{
		return mExtra;
	}
	
}
}
