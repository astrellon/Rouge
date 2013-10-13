#include "ui_item_description.h"

#include <gfx/gfx_engine.h>
#include <gfx/gfx_texture.h>
#include <gl.h>

#include <sstream>

namespace am {
namespace ui {
	// CHANGE TO EXTEND FROM TOOLTIP!
	ItemDescription::ItemDescription() :
		mBackground(new Sprite("ui:tooltip")),
		mText(new TextField2()),
		mItemSprite(new Sprite())
	{
		addChild(mBackground);
		addChild(mText);
		addChild(mItemSprite);
	}
	ItemDescription::ItemDescription(Item *item) :
		mBackground(new Sprite("ui:tooltip")),
		mItem(item),
		mText(new TextField2()),
		mItemSprite(new Sprite())
	{
		addChild(mBackground);
		addChild(mText);
		addChild(mItemSprite);
	}
	ItemDescription::~ItemDescription()
	{
		
	}

	void ItemDescription::show()
	{
		GfxEngine::getEngine()->getTooltipLayer()->addChild(this);
	}
	void ItemDescription::hide()
	{
		GfxEngine::getEngine()->getTooltipLayer()->removeChild(this);
	}

	void ItemDescription::setItem(Item *item)
	{
		mItem = item;
		if (mItem->getGraphic())
		{
			mItemSprite->setAsset(mItem->getGraphic()->getAsset());
		}
	}
	Item *ItemDescription::getItem() const
	{
		return mItem;
	}

	void ItemDescription::updateText()
	{
		stringstream ss;
		if (mItem)
		{
			ss << "Name: " << mItem->getFullItemName()
				<< "\n-----------";
		}
		mText->setText(ss.str());
	}

}
}
