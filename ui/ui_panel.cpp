#include "ui_panel.h"

#include <ui/mouse_event.h>

namespace am {
namespace ui {

	Panel::Panel()
	{
		mBack = new Sprite("uiPanel");
		addChild(mBack);

		mTitle = new TextField();
		addChild(mTitle);
	}
	Panel::~Panel()
	{
	}

	void Panel::setWidth(float width)
	{
		mBack->setWidth(width);
		mTitle->setWidth(width);
		UIComponent::setWidth(width);
	}
	void Panel::setHeight(float height)
	{
		mBack->setHeight(height);
		UIComponent::setHeight(height);
	}

	void Panel::onEvent(MouseEvent *e)
	{

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
		return mTitle->getText().c_str();
	}

	Sprite *Panel::getBackSprite()
	{
		return mBack;
	}

}
}
