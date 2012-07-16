#include "ui_button.h"

#include <gfx/gfx_renderable.h>
#include <gfx/gfx_asset.h>
#include <gfx/gfx_sprite.h>
#include <gfx/gfx_text_field.h>
#include <gfx/gfx_font.h>

#include <gfx/gfx_colour_effect.h>

#include <base/handle.h>
using namespace am::util;

#include "mouse_manager.h"

namespace am {
namespace ui {

	Button::Button() :
		UIComponent(),
		mHitbox(NULL)
	{
		mGraphic = new Sprite();

		init();
	}
	Button::Button(const char *assetName, Renderable *hitbox) :
		UIComponent(),
		mHitbox(hitbox)
	{
		mGraphic = new Sprite(assetName);

		init();
	}
	Button::Button(Asset *asset, Renderable *hitbox) :
		UIComponent(),
		mHitbox(hitbox)
	{
		mGraphic = new Sprite(asset);
		
		init();
	}
	Button::Button(const char *assetName, const char *label, Renderable *hitbox) :
		UIComponent(),
		mHitbox(hitbox)
	{
		mGraphic = new Sprite(assetName);

		init();

		setLabel(label);
	}
	Button::Button(Asset *asset, const char *label, Renderable *hitbox) :
		UIComponent(),
		mHitbox(hitbox)
	{
		mGraphic = new Sprite(asset);
		
		init();

		setLabel(label);
	}
	Button::~Button()
	{
		if (mHitbox.get())
		{
			removeListeners(mHitbox.get());
		}
		else
		{
			removeListeners(this);
		}
	}

	void Button::init()
	{
		mGraphic->setInteractive(true);
		addChild(mGraphic.get());

		setHitbox(mHitbox.get());

		mLabel = new TextField();
		mLabel->setBaseFont("arial");
		mLabel->setColour(0.6f, 0.8f, 1.0f);
		addChild(mLabel.get());

		mLabel->setWidth(mGraphic->getWidth());
		mLabel->setAlignment(TextField::ALIGN_CENTER);
	}

	void Button::setHitbox(Renderable *hitbox)
	{
		if (mHitbox.get())
		{
			removeListeners(mHitbox.get());
		}
		else
		{
			removeListeners(mGraphic.get());
		}
		mHitbox = hitbox;
		if (mHitbox.get())
		{
			addListeners(mHitbox.get());
			mHitbox->setInteractive(true);
		}
		else
		{
			addListeners(mGraphic.get());
		}
	}
	Renderable *Button::getHitbox()
	{
		return mHitbox.get();
	}

	void Button::setLabel(const char *label)
	{
		mLabel->setText(label);
		updateLabelPos();
	}
	void Button::setLabel(const string &label)
	{
		mLabel->setText(label);
		updateLabelPos();
	}
	string Button::getLabel() const
	{
		return mLabel->getText();
	}

	void Button::onEvent(MouseEvent *e)
	{
		Handle<Event> clickEvent;
		switch (e->getMouseEventType())
		{
		default:
		case am::ui::MOUSE_OUT:
			mGraphic->setCurrentFrame(0);
			break;
		case am::ui::MOUSE_MOVE:
			{
				int frame = 1;
				if (MouseManager::getManager()->getButtonDown(e->getMouseButton()))
				{
					frame = 2;
				}
				mGraphic->setCurrentFrame(frame);
			}
			break;
		case am::ui::MOUSE_UP:
			clickEvent = new Event("click", this);
			fireEvent(clickEvent.get());
		case am::ui::MOUSE_OVER:
			mGraphic->setCurrentFrame(1);
			break;
		case am::ui::MOUSE_DOWN:
			mGraphic->setCurrentFrame(2);
			break;
		}
	}

	void Button::setWidth(float width)
	{
		if (mGraphic.get())
		{
			mGraphic->setWidth(width);
			mLabel->setWidth(width);
			updateLabelPos();
		}
	}
	float Button::getWidth()
	{
		if (mGraphic.get())
		{
			return mGraphic->getWidth();
		}
		return 0.0f;
	}

	void Button::setHeight(float height)
	{
		if (mGraphic.get())
		{
			mGraphic->setHeight(height);
			mLabel->setHeight(height);
			updateLabelPos();
		}
	}
	float Button::getHeight()
	{
		if (mGraphic.get())
		{
			return mGraphic->getHeight();
		}
		return 0.0f;
	}

	void Button::setEnabled(bool enabled)
	{
		UIComponent::setEnabled(enabled);
		if (enabled)
		{
			mGraphic->setCurrentFrame(0);
		}
		else
		{
			mGraphic->setCurrentFrame(3);
		}
	}

	void Button::addListeners(Renderable *target)
	{
		target->addEventListener("mouse_down", this);
		target->addEventListener("mouse_move", this);
		target->addEventListener("mouse_up", this);
		target->addEventListener("mouse_out", this);
		target->addEventListener("mouse_over", this);
	}
	void Button::removeListeners(Renderable *target)
	{
		target->removeEventListener("mouse_down", this);
		target->removeEventListener("mouse_move", this);
		target->removeEventListener("mouse_up", this);
		target->removeEventListener("mouse_out", this);
		target->removeEventListener("mouse_over", this);
	}

	void Button::updateLabelPos()
	{
		float fontHeight = mLabel->getMeasuredHeight();
		mLabel->setPosition(0.0f, (mGraphic->getHeight() - fontHeight) * 0.5f);
	}

}
}
