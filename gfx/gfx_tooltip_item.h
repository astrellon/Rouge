#pragma once

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_text_field2.h>
#include <gfx/gfx_sprite.h>
#include <gfx/gfx_tooltip.h>
using namespace am::gfx;

#include <string>
using namespace std;

#include <game/item.h>

namespace am {
namespace gfx {

	class TooltipItem : public Tooltip {
	public:
		TooltipItem();
		TooltipItem(const Item *item);
		TooltipItem(const TooltipItem &copy);
		~TooltipItem();

		virtual Renderable *clone() const;

		virtual void setItem(const Item *item);
		virtual const Item *getItem() const;

		void updateTooltip();

	protected:

		Handle<const Item> mItem;
		Handle<Layer> mLayer;
		Handle<Sprite> mGraphic;
		Handle<TextField2> mText;

		void init();
	};

}
}
