#include "gfx_scrollbar.h"

#include "gfx_engine.h"
#include "gfx_texture.h"
#include "gfx_asset.h"
#include "gfx_layer.h"

#include <ui/mouse_manager.h>

#include <log/logger.h>

namespace am {
namespace gfx {

	const char *Scrollbar::SCROLL_VALUE_CHANGE = "scroll_value_change";

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
		mBar->addEventListener(ui::Mouse::MOUSE_DOWN, this);

		mWidth = max(mBtnUp->getWidth(), max(mBtnDown->getWidth(), mBar->getWidth()));

		Sprite *sprite = new Sprite("ui:scrollbar_up");
		addChild(sprite);
		
		if (backAsset != nullptr && backAsset[0] != '\0')
		{
			mBack = new Sprite(backAsset);
			addChild(mBack);
		}
		else
		{
			mBack = nullptr;
		}
		addChild(mBtnUp);
		addChild(mBtnDown);
		addChild(mBar);

		updateBar();
	}
	Scrollbar::~Scrollbar()
	{

	}

	void Scrollbar::onEvent(ui::Event *e)
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
	void Scrollbar::onEvent(ui::MouseEvent *e)
	{
		ui::MouseManager *manager = ui::MouseManager::getManager();
		if (e->getEventTarget() == mBar.get() && e->getMouseEventType() == ui::Mouse::MOUSE_DOWN)
		{
			ui::MouseManager *manager = ui::MouseManager::getManager();
			manager->setDragOffset(0, e->getLocalMouseY());
			manager->addEventListener(ui::Mouse::MOUSE_MOVE, this);
			manager->addEventListener(ui::Mouse::MOUSE_UP, this);
		}
		if (e->getMouseEventType() == ui::Mouse::MOUSE_MOVE)
		{
			// We don't care about the mouseX value.
			float mouseX = 0.0f;
			float mouseY = static_cast<float>(manager->getMouseY());
			float outMouseX = 0.0f;
			float outMouseY = 0.0f;
			getScreenToLocal(mouseX, mouseY, outMouseX, outMouseY);

			outMouseY -= mBtnUp->getHeight() + manager->getDragOffsetY();

			float scrollHeight = mHeight - mBtnUp->getHeight() - mBtnDown->getHeight() - mBar->getHeight();
			float range = static_cast<float>(mMaxValue - mMinValue);
			float percent = outMouseY / scrollHeight;
			setValue(static_cast<int>(percent * range));
		}
		else if (e->getMouseEventType() == ui::Mouse::MOUSE_UP)
		{
			manager->removeEventListener(ui::Mouse::MOUSE_MOVE, this);
			manager->removeEventListener(ui::Mouse::MOUSE_UP, this);
		}
	}

	void Scrollbar::render(float dt)
	{
		if (mVisible)
		{
			Layer::render(dt);
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
		bool changed = mMaxValue != value;
		mMinValue = value;
		if (mValue < value)
		{
			mValue = value;
			changed = true;
		}
		if (changed)
		{
			updateBar();
		}
	}
	
	int Scrollbar::getMaxValue() const
	{
		return mMaxValue;
	}
	void Scrollbar::setMaxValue(int value)
	{
		bool changed = mMaxValue != value;
		mMaxValue = value;
		if (mValue > value)
		{
			mValue = value;
			changed = true;
		}
		if (changed)
		{
			updateBar();
		}
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
			base::Handle<ui::Event> e(new ui::Event(SCROLL_VALUE_CHANGE, this));
			fireEvent<ui::Event>(e);
		}
	}

	void Scrollbar::updateBar()
	{
		if (mBtnUp.get() == nullptr || mBtnDown.get() == nullptr || mBar.get() == nullptr)
		{
			return;
		}
		int value = mValue;
		if (mMaxValue < mMinValue)
		{
			value = mMinValue;
		}
		float scrollHeight = mHeight - mBtnUp->getHeight() - mBtnDown->getHeight() - mBar->getHeight();
		float range = static_cast<float>(mMaxValue - mMinValue);
		float percent = static_cast<float>(value) / range;
		mBar->setPosition(0.0f, percent * scrollHeight + mBtnUp->getHeight());
		mBtnDown->setPosition(0.0f, mHeight - mBtnDown->getHeight());
		if (mBack.get() != nullptr)
		{
			mBack->setPosition(0.0f, mBtnUp->getHeight());
			float backHeight = mHeight - mBtnUp->getHeight() - mBtnDown->getHeight();
			mBack->setHeight(backHeight);
		}
	}
	
}
}
