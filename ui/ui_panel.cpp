#include "ui_panel.h"

#include <log/logger.h>

#include <ui/mouse_event.h>
#include <ui/mouse_manager.h>

#include <sstream>

namespace am {
namespace ui {

	Panel::Panel() :
		UIComponent(),
		mStartingWidth(100.0f),
		mStartingHeight(100.0f),
		mResizing(false)
	{
		mBack = new Sprite("uiPanel");
		mBack->setInteractive(true);
		addChild(mBack);

		mTitle = new TextField();
		addChild(mTitle);
		mTitle->setPosition(9, 7);

		mBack->addEventListener(MOUSE_DOWN, this);

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
		if (e->getMouseEventType() == MOUSE_DOWN)
		{
			if (e->getLocalMouseY() < 20)
			{
				manager->setDragOffset(e->getLocalMouseX(), e->getLocalMouseY());
				manager->addEventListener(MOUSE_MOVE, this);
				manager->addEventListener(MOUSE_UP, this);
			}
			else
			{
				if (e->getLocalMouseX() > mWidth - 20 &&
					e->getLocalMouseY() > mHeight - 20)
				{
					mResizing = true;
					manager->setDragOffset(e->getLocalMouseX(), e->getLocalMouseY());
					manager->addEventListener(MOUSE_MOVE, this);
					manager->addEventListener(MOUSE_UP, this);
					mStartingWidth = mWidth;
					mStartingHeight = mHeight;
				}
			}
		}
		else if (e->getMouseEventType() == MOUSE_MOVE)
		{
			if (mResizing)
			{
				float localX = 0.0f;
				float localY = 0.0f;
				getScreenToLocal(e->getMouseX(), e->getMouseY(), localX, localY);
				
				float dx = localX - manager->getDragOffsetX();
				float dy = localY - manager->getDragOffsetY();
				setSize(mStartingWidth + dx, mStartingHeight + dy);
			}
			else
			{
				int x = e->getMouseX() - manager->getDragOffsetX();
				int y = e->getMouseY() - manager->getDragOffsetY();
				setParentOffset(x, y);
			}
		}
		else if (e->getMouseEventType() == MOUSE_UP)
		{
			manager->removeEventListener(MOUSE_MOVE, this);
			manager->removeEventListener(MOUSE_UP, this);
			mResizing = false;
		}
	}

	TextField *Panel::getTitleField()
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

	Sprite *Panel::getBackSprite()
	{
		return mBack;
	}

}
}
