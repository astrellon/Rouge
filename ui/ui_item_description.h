#pragma once

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_text_field2.h>
#include <gfx/gfx_sprite.h>
#include <gfx/gfx_layer.h>
using namespace am::gfx;

#include <string>
using namespace std;

#include <game/item.h>
using namespace am::game;

namespace am {
namespace ui {

	class ItemDescription : public Layer {
	public:
		ItemDescription();
		ItemDescription(Item *item);
		~ItemDescription();

		virtual void show();
		virtual void hide();

		virtual void setItem(Item *item);
		virtual Item *getItem() const;

	protected:

		Handle<Item> mItem;
		Handle<Sprite> mItemSprite;
		Handle<Sprite> mBackground;
		Handle<TextField2> mText;

		void updateText();
	};

}
}
