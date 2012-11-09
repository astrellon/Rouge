#include "gfx_scrollbar.h"

#include "gfx_engine.h"
#include "gfx_texture.h"
#include "gfx_asset.h"
#include "gfx_layer.h"

#include <ui/mouse_manager.h>
using am::ui::MouseManager;

#include <log/logger.h>

namespace am {
namespace gfx {

	Scrollbar::Scrollbar(const char *btnUpAsset, const char *btnDownAsset, const char *barAsset, const char *backAsset) :
		Layer(),
		mMinValue(0),
		mMaxValue(100),
		mValue(0)
	{
		setInteractive(true);

		mBtnUp = new Button(btnUpAsset);
		mBtnUp->addEventListener("click", this);
		mBtnDown = new Button(btnDownAsset);
		mBtnDown->addEventListener("click", this);
		mBar = new Button(barAsset);
		mBar->addEventListener(MOUSE_DOWN, this);
		
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

	void Scrollbar::onEvent(Event *e)
	{
		if (e->getType().compare("click") == 0)
		{
			if (e->getEventTarget() == mBtnUp.get())
			{
				setValue(getValue() - 1);
			}
			else if (e->getEventTarget() == mBtnDown.get())
			{
				setValue(getValue() + 1);
			}
		}
	}
	void Scrollbar::onEvent(MouseEvent *e)
	{
		MouseManager *manager = MouseManager::getManager();
		if (e->getEventTarget() == mBar.get() && e->getMouseEventType() == MOUSE_DOWN)
		{
			MouseManager *manager = MouseManager::getManager();
			manager->setDragOffset(0.0f, e->getLocalMouseY());
			manager->addEventListener(MOUSE_MOVE, this);
			manager->addEventListener(MOUSE_UP, this);
		}
		if (e->getMouseEventType() == MOUSE_MOVE)
		{
			// We don't care about the mouseX value.
			float mouseX = 0.0f;
			float mouseY = manager->getMouseY();
			float outMouseX = 0.0f;
			float outMouseY = 0.0f;
			getScreenToLocal(mouseX, mouseY, outMouseX, outMouseY);

			outMouseY -= mBtnUp->getHeight() + manager->getDragOffsetY();

			float scrollHeight = mHeight - mBtnUp->getHeight() - mBtnDown->getHeight() - mBar->getHeight();
			float range = static_cast<float>(mMaxValue - mMinValue);
			float percent = outMouseY / scrollHeight;
			setValue(static_cast<int>(percent * range));
		}
		else if (e->getMouseEventType() == MOUSE_UP)
		{
			manager->removeEventListener(MOUSE_MOVE, this);
			manager->removeEventListener(MOUSE_UP, this);
		}
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
		if (mValue < value)
		{
			mValue = value;
		}
		updateBar();
	}
	
	int Scrollbar::getMaxValue() const
	{
		return mMaxValue;
	}
	void Scrollbar::setMaxValue(int value)
	{
		mMaxValue = value;
		if (mValue > value)
		{
			mValue = value;
		}
		updateBar();
	}
	
	int Scrollbar::getValue() const
	{
		return mValue;
	}
	void Scrollbar::setValue(int value)
	{
		if (value > mMaxValue)
		{
			value = mMaxValue;
		}
		if (value < mMinValue)
		{
			value = mMinValue;
		}
		if (value != mValue)
		{
			mValue = value;
			updateBar();
			Handle<Event> e(new Event("scroll_value_change", this));
			fireEvent<Event>(e);
		}
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
