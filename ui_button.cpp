#include "ui_button.h"

#include "gfx/gfx_engine.h"
#include "gfx/gfx_renderable.h"
#include "gfx/gfx_asset.h"

namespace am {
namespace ui {

	Button::Button(GfxEngine *engine, Renderable *hitbox) :
		Sprite(engine),
		mHitbox(hitbox)
	{
		init();
	}
	Button::Button(GfxEngine *engine, const char *assetName, Renderable *hitbox) :
		Sprite(engine, assetName),
		mHitbox(hitbox)
	{
		init();
	}
	Button::Button(GfxEngine *engine, Asset *asset, Renderable *hitbox) :
		Sprite(engine, asset),
		mHitbox(hitbox)
	{
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
		setNumFramesX(2);
		setNumFramesY(2);
		if (mHitbox)
		{
			addListeners(mHitbox);
		}
		else
		{
			addListeners(this);
		}
	}

	void Button::setHitbox(Renderable *hitbox)
	{
		if (mHitbox)
		{
			removeListeners(mHitbox);
		}
		else
		{
			removeListeners(this);
		}
		mHitbox = hitbox;
		if (mHitbox)
		{
			addListeners(mHitbox);
		}
		else
		{
			addListeners(this);
		}
	}
	Renderable *Button::getHitbox()
	{
		return mHitbox;
	}

	void Button::onEvent(MouseEvent *e)
	{
		switch (e->getMouseEventType())
		{
		default:
		case am::ui::MOUSE_OUT:
			setCurrentFrame(0);
			break;
		case am::ui::MOUSE_OVER:
		case am::ui::MOUSE_UP:
			setCurrentFrame(1);
			break;
		case am::ui::MOUSE_DOWN:
			setCurrentFrame(2);
			break;
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
