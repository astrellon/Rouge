#include "ui_button.h"

#include "gfx/gfx_engine.h"
#include "gfx/gfx_renderable.h"
#include "gfx/gfx_asset.h"
#include "gfx/gfx_sprite.h"
#include "gfx/gfx_text_field.h"
#include "gfx/gfx_font.h"

#include "mouse_manager.h"

namespace am {
namespace ui {

	Button::Button(GfxEngine *engine, Renderable *hitbox) :
		UIComponent(engine),
		mHitbox(hitbox)
	{
		mGraphic = new Sprite(engine);

		init();
	}
	Button::Button(GfxEngine *engine, const char *assetName, Renderable *hitbox) :
		UIComponent(engine),
		mHitbox(hitbox)
	{
		mGraphic = new Sprite(engine, assetName);

		init();
	}
	Button::Button(GfxEngine *engine, Asset *asset, Renderable *hitbox) :
		UIComponent(engine),
		mHitbox(hitbox)
	{
		mGraphic = new Sprite(engine, asset);
		
		init();
	}
	Button::~Button()
	{
		if (mHitbox)
		{
			removeListeners(mHitbox);
		}
		else
		{
			removeListeners(this);
		}
	}

	void Button::init()
	{
		mGraphic->setEnableInteractive(true);
		addChild(mGraphic);

		mGraphic->setNumFramesX(2);
		mGraphic->setNumFramesY(2);
		mGraphic->setNumTotalFrames(4);
		setHitbox(mHitbox);
		setEnableInteractive(true);

		mLabel = new TextField(mGfxEngine);
		mLabel->setBaseFont("arial");
		mLabel->setText("Multiline\nButton");
		mLabel->setColour(0.6f, 0.8f, 1.0f);
		addChild(mLabel);

		mLabel->setWidth(mGraphic->getWidth());
		mLabel->setAlignment(TextField::ALIGN_CENTER);
		float fontHeight = mLabel->getMeasuredHeight();
		mLabel->getTransform().setPosition(Vector4f(0, (mGraphic->getHeight() - fontHeight) * 0.5f, 0));
	}

	void Button::setHitbox(Renderable *hitbox)
	{
		if (mHitbox)
		{
			removeListeners(mHitbox);
		}
		else
		{
			removeListeners(mGraphic);
		}
		mHitbox = hitbox;
		if (mHitbox)
		{
			addListeners(mHitbox);
			mHitbox->setEnableInteractive(true);
		}
		else
		{
			addListeners(mGraphic);
		}
	}
	Renderable *Button::getHitbox()
	{
		return mHitbox;
	}

	void Button::setLabel(const char *label)
	{
		mLabel->setText(label);
	}
	void Button::setLabel(const string &label)
	{
		mLabel->setText(label);
	}
	string Button::getLabel() const
	{
		return mLabel->getText();
	}

	void Button::onEvent(MouseEvent *e)
	{
		switch (e->getMouseEventType())
		{
		default:
		case am::ui::MOUSE_OUT:
			mGraphic->setCurrentFrame(0);
			break;
		case am::ui::MOUSE_MOVE:
			if (e->getManager()->getButtonDown(e->getMouseButton()))
			{
				mGraphic->setCurrentFrame(2);
			}
			else
			{
				mGraphic->setCurrentFrame(1);
			}
			break;
		case am::ui::MOUSE_OVER:
		case am::ui::MOUSE_UP:
			mGraphic->setCurrentFrame(1);
			break;
		case am::ui::MOUSE_DOWN:
			mGraphic->setCurrentFrame(2);
			break;
		}
	}

	float Button::getWidth()
	{
		if (mGraphic)
		{
			return mGraphic->getWidth();
		}
	}
	float Button::getHeight()
	{
		if (mGraphic)
		{
			return mGraphic->getHeight();
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

}
}
