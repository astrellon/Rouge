#include "ui_text_button.h"

#include <gfx/gfx_renderable.h>
#include <gfx/gfx_asset.h>
#include <gfx/gfx_sprite.h>
#include <gfx/gfx_text_field.h>
#include <gfx/gfx_font.h>
#include <gfx/gfx_button.h>

#include <gfx/gfx_colour_effect.h>

#include <base/handle.h>
using namespace am::util;

#include "mouse_manager.h"

namespace am {
namespace ui {

	TextButton::TextButton() :
		UIComponent()
	{
		mGraphic = new Button();

		init();
	}
	TextButton::TextButton(const char *assetName, Renderable *hitbox) :
		UIComponent()
	{
		mGraphic = new Button(assetName, hitbox);

		init();
	}
	TextButton::TextButton(const char *assetName, const char *label, Renderable *hitbox) :
		UIComponent()
	{
		mGraphic = new Button(assetName, hitbox);

		init();

		setLabel(label);
	}
	TextButton::~TextButton()
	{
		
	}

	void TextButton::init()
	{
		addChild(mGraphic);
		mGraphic->addEventListener("click", this);

		mLabel = new TextField();
		mLabel->setBaseFont("default:arial");
		mLabel->setGfxComponent(new GfxComponent());
		mLabel->getGfxComponent()->setColour(0.6f, 0.8f, 1.0f);
		addChild(mLabel);

		mLabel->setWidth(mGraphic->getWidth());
		mLabel->setAlignment(TextField::ALIGN_CENTER);
	}

	void TextButton::setHitbox(Renderable *hitbox)
	{
		mGraphic->setHitbox(hitbox);
	}
	Renderable *TextButton::getHitbox()
	{
		return mGraphic->getHitbox();
	}

	void TextButton::setLabel(const char *label)
	{
		mLabel->setText(label);
		updateLabelPos();
	}
	void TextButton::setLabel(const string &label)
	{
		mLabel->setText(label);
		updateLabelPos();
	}
	string TextButton::getLabel() const
	{
		return mLabel->getText();
	}

	TextField *TextButton::getLabelField() const
	{
		return mLabel;
	}

	void TextButton::setWidth(float width)
	{
		if (mGraphic.get())
		{
			width = clampWidth(width);
			mGraphic->setWidth(width);
			mLabel->setWidth(width);
			updateLabelPos();
		}
	}
	float TextButton::getWidth()
	{
		if (mGraphic.get())
		{
			return mGraphic->getWidth();
		}
		return 0.0f;
	}

	void TextButton::setHeight(float height)
	{
		if (mGraphic.get())
		{
			height = clampHeight(height);
			mGraphic->setHeight(height);
			mLabel->setHeight(height);
			updateLabelPos();
		}
	}
	float TextButton::getHeight()
	{
		if (mGraphic.get())
		{
			return mGraphic->getHeight();
		}
		return 0.0f;
	}

	void TextButton::setEnabled(bool enabled)
	{
		UIComponent::setEnabled(enabled);
		mGraphic->setEnabled(enabled);
	}

	void TextButton::onEvent(Event *e)
	{
		Handle<Event> refire(new Event(e->getType().c_str(), this));
		fireEvent<Event>(refire);
	}

	void TextButton::updateLabelPos()
	{
		float fontHeight = mLabel->getMeasuredHeight();
		mLabel->setPosition(0.0f, (mGraphic->getHeight() - fontHeight) * 0.5f);
	}

}
}
