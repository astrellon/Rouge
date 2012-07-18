#include "ui_panel.h"

#include <log/logger.h>

#include <ui/mouse_event.h>
#include <ui/mouse_manager.h>

#include <sstream>

namespace am {
namespace ui {

	Panel::Panel()
	{
		mBack = new Sprite("uiPanel");
		mBack->setInteractive(true);
		addChild(mBack);

		mTitle = new TextField();
		addChild(mTitle);
		mTitle->setPosition(9, 7);

		mBack->addEventListener(MOUSE_DOWN, this);
	}
	Panel::~Panel()
	{
	}

	void Panel::setWidth(float width)
	{
		mBack->setWidth(width);
		mTitle->setWidth(width - 18.0f);
		UIComponent::setWidth(width);
	}
	void Panel::setHeight(float height)
	{
		mBack->setHeight(height);
		UIComponent::setHeight(height);
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
				}
			}
		}
		else if (e->getMouseEventType() == MOUSE_MOVE)
		{
			if (mResizing)
			{

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
