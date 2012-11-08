#include "gfx_scrollbar.h"

#include "gfx_engine.h"
#include "gfx_texture.h"
#include "gfx_asset.h"
#include "gfx_layer.h"

#include <log/logger.h>

namespace am {
namespace gfx {

	Scrollbar::Scrollbar(const char *btnUpAsset, const char *btnDownAsset, const char *barAsset, const char *backAsset) :
		Layer(),
		mMinValue(0),
		mMaxValue(100),
		mValue(0)
	{
		mBtnUp = new Sprite(btnUpAsset);
		mBtnDown = new Sprite(btnDownAsset);
		mBar = new Sprite(barAsset);
		
		if (backAsset != NULL && backAsset[0] != '\0')
		{
			mBack = new Sprite(backAsset);
			addChild(mBack);
		}
		else
		{
			mBack = NULL;
		}
		addChild(mBtnUp);
		addChild(mBtnDown);
		addChild(mBar);

		updateBar();
	}
	Scrollbar::~Scrollbar()
	{

	}

	void Scrollbar::setWidth(float width)
	{
		Layer::setWidth(width);
		updateBar();
	}
	void Scrollbar::setHeight(float height)
	{
		Layer::setHeight(height);
		updateBar();
	}

	int Scrollbar::getMinValue() const
	{
		return mMinValue;
	}
	void Scrollbar::setMinValue(int value)
	{
		mMinValue = value;
		updateBar();
	}
	
	int Scrollbar::getMaxValue() const
	{
		return mMaxValue;
	}
	void Scrollbar::setMaxValue(int value)
	{
		mMaxValue = value;
		updateBar();
	}
	
	int Scrollbar::getValue() const
	{
		return mValue;
	}
	void Scrollbar::setValue(int value)
	{
		mValue = value;
		updateBar();
	}

	void Scrollbar::updateBar()
	{
		if (mBtnUp.get() == NULL || mBtnDown.get() == NULL || mBar.get() == NULL)
		{
			return;
		}
		float scrollHeight = mHeight - mBtnUp->getHeight() - mBtnDown->getHeight() - mBar->getHeight();
		float range = static_cast<float>(mMaxValue - mMinValue);
		float percent = static_cast<float>(mValue) / range;
		mBar->setPosition(0.0f, percent * scrollHeight + mBtnUp->getHeight());
		mBtnDown->setPosition(0.0f, mHeight - mBtnDown->getHeight());
		if (mBack.get() != NULL)
		{
			mBack->setPosition(0.0f, mBtnUp->getHeight());
			float backHeight = mHeight - mBtnUp->getHeight() - mBtnDown->getHeight();
			mBack->setHeight(backHeight);
		}
	}
	
}
}
