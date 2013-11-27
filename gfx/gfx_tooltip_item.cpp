#include "gfx_tooltip_item.h"

#include <gfx/gfx_engine.h>
#include <gfx/gfx_texture.h>
#include <gl.h>

namespace am {
namespace gfx {

	TooltipItem::TooltipItem() :
		Tooltip(),
		mGraphic(new Sprite()),
		mText(new TextField2()),
		mLayer(new Layer()),
		mItem(nullptr)
	{
		init();
	}
	TooltipItem::TooltipItem(const Item *item) :
		Tooltip(item),
		mGraphic(new Sprite()),
		mText(new TextField2()),
		mLayer(new Layer())
	{
		init();

		setItem(item);
	}
	TooltipItem::TooltipItem(const TooltipItem &copy) :
		Tooltip(copy)
	{
		mLayer = dynamic_cast<Layer *>(mChildren[1].get());
		mGraphic = dynamic_cast<Sprite *>(mLayer->getChildAt(0));
		mText = dynamic_cast<TextField2 *>(mLayer->getChildAt(1));

		setItem(copy.mItem);
	}
	TooltipItem::~TooltipItem()
	{
		
	}

	Renderable *TooltipItem::clone() const
	{
		return new TooltipItem(*this);
	}

	void TooltipItem::setItem(const Item *item)
	{
		if (mItem != item)
		{
			mItem = item;
			updateTooltip();
		}
	}
	const Item *TooltipItem::getItem() const
	{
		return mItem;
	}

	void TooltipItem::init()
	{
		addChild(mLayer);
		mLayer->addChild(mGraphic);
		mLayer->addChild(mText);

		mText->setPosition(8.0f, 8.0f);
		mGraphic->setPosition(8.0f, 8.0f);

		setDelays(0.0f, 0.0f);

		mTooltipGraphic = mLayer;
	}

	void  TooltipItem::updateTooltip()
	{
		std::stringstream ss;
		if (mItem->getItemLocation() == Item::INVENTORY)
		{
			setDelays(0.0f, 0.0f);
			mGraphic->setAsset(mItem->getGraphic()->getAsset());
			mGraphic->setVisible(true);
			mText->setPosition(16.0f + mGraphic->getWidth(), 8.0f);
			ItemCommon::ItemType type = mItem->getItemType();
			if (type == ItemCommon::GOLD)
			{
				ss << "Gold: " << mItem->getItemValue() << '\n';
			}
			else if (type == ItemCommon::POTION)
			{
				ss << "Value: " << mItem->getItemValue();
			}
			else if (type == ItemCommon::SPELL_SCROLL)
			{

			}
			else if (type == ItemCommon::MESSAGE_SCROLL)
			{
				ss << "Right click to read the message.";
			}
			else
			{
				int i = 0;
				while (true)
				{
					Stat::StatType stat = Stat::getStatType(i);
					i++;
					if (stat == Stat::MAX_STAT_LENGTH)
					{
						break;
					}
					float value = mItem->getStatModifiers()->calculateStat(stat, 0.0f);
					if (value <= 0.00001f)
					{
						continue;
					}
					ss << "<title class='" << Stat::getStatName(stat) << "'>" << Stat::getNiceStatName(stat) << "</title>: " << value << '\n';
					
				}
				ss << "<title class='value'>Value</title>: " << mItem->getItemValue();
			}
			mText->setText(ss.str().c_str());

			float width = mText->getMeasuredWidth();
			float height = mText->getMeasuredHeight();
			mLayer->setWidth(mText->getPositionX() + width);
			mLayer->setHeight(max(height, mGraphic->getHeight()));
		}
		else
		{
			setDelays(0.8f, 1.5f);
			mGraphic->setVisible(false);
			mText->setPosition(8.0f, 8.0f);
			std::stringstream ss;
			ss << mItem->getFullItemName() << '\n';
			ss << "Looks to be a " << ItemCommon::getItemTypeNameNice(mItem->getItemType());
			mText->setText(ss.str().c_str());

			float width = mText->getMeasuredWidth();
			float height = mText->getMeasuredHeight();
			mLayer->setSize(width, height);
		}
	}

}
}
