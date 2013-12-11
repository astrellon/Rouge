#include "ui_panel.h"

#include <log/logger.h>

#include <ui/mouse_event.h>
#include <ui/mouse_manager.h>
#include <ui/ui_debug_inspector.h>

#include <sstream>

namespace am {
namespace ui {

	Panel::Panel() :
		UIComponent(),
		mStartingWidth(100.0f),
		mStartingHeight(100.0f),
		mResizing(false),
		mDragX(0.0f),
		mDragY(0.0f)
	{
        setVisible(false);

		mBack = new gfx::Sprite("ui:panel");
		mBack->setName("Panel Back");
		mBack->setInteractive(true);
		addChild(mBack);

		mTitle = new gfx::TextField();
		addChild(mTitle);
		mTitle->setPosition(9, 7);

		mBack->addEventListener(Mouse::MOUSE_DOWN, this);

		setMinWidth(100.0f);
		setMinHeight(100.0f);
		setMaxWidth(600.0f);
		setMaxHeight(400.0f);
	}
	Panel::~Panel()
	{
	}

	void Panel::setWidth(float width)
	{
		UIComponent::setWidth(width);
		mBack->setWidth(mWidth);
		mTitle->setWidth(mWidth - 18.0f);
	}
	void Panel::setHeight(float height)
	{
		UIComponent::setHeight(height);
		mBack->setHeight(mHeight);
	}

	void Panel::onEvent(MouseEvent *e)
	{
		MouseManager *manager = MouseManager::getManager();
		if (e->getMouseEventType() == Mouse::MOUSE_DOWN)
		{
			if (e->getLocalMouseY() < 20)
			{
				mDragX = e->getLocalMouseX();
				mDragY = e->getLocalMouseY();
				manager->addEventListener(Mouse::MOUSE_MOVE, this);
				manager->addEventListener(Mouse::MOUSE_UP, this);
			}
			else
			{
				if (e->getLocalMouseX() > mWidth - 20 &&
					e->getLocalMouseY() > mHeight - 20)
				{
					mResizing = true;
					mDragX = e->getLocalMouseX();
					mDragY = e->getLocalMouseY();
					manager->addEventListener(Mouse::MOUSE_MOVE, this);
					manager->addEventListener(Mouse::MOUSE_UP, this);
					mStartingWidth = mWidth;
					mStartingHeight = mHeight;
				}
			}
			e->stopPropagation();
		}
		else if (e->getMouseEventType() == Mouse::MOUSE_MOVE)
		{
			float localX = 0.0f;
			float localY = 0.0f;
			getScreenToLocal(static_cast<float>(e->getMouseX()), static_cast<float>(e->getMouseY()), localX, localY);
				
			float dx = localX - mDragX;
			float dy = localY - mDragY;
			if (mResizing)
			{
				if (mAnchorX == X_RIGHT)
				{
					dx *= 0.5f;
				}
				setSize(mStartingWidth + dx, mStartingHeight + dy);
			}
			else
			{
				setParentOffset(mParentOffsetX + dx, mParentOffsetY + dy);
			}
		}
		else if (e->getMouseEventType() == Mouse::MOUSE_UP)
		{
			manager->removeEventListener(Mouse::MOUSE_MOVE, this);
			manager->removeEventListener(Mouse::MOUSE_UP, this);
			mResizing = false;
		}
	}

	gfx::TextField *Panel::getTitleField()
	{
		return mTitle;
	}
	void Panel::setTitle(const char *title)
	{
		mTitle->setText(title);
	}
	const char *Panel::getTitle() const
	{
		return mTitle->getText();
	}

	gfx::Sprite *Panel::getBackSprite()
	{
		return mBack;
	}

    void Panel::show()
    {
        setVisible(true);
    }
    void Panel::hide()
    {
        setVisible(false);
    }

}
}

