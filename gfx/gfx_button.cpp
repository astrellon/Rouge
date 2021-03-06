#include "gfx_button.h"

#include "gfx_renderable.h"

#include <ui/mouse_manager.h>

#include <log/logger.h>

namespace am {
namespace gfx {

	Button::Button(Renderable *hitbox) :
		Sprite(),
		mHitbox(hitbox),
		mEnabled(true)
	{
		setHitbox(hitbox);
		setInteractive(true);
	}
	Button::Button(const char *assetName, Renderable *hitbox) :
		Sprite(assetName),
		mHitbox(hitbox),
		mEnabled(true)
	{
		setHitbox(hitbox);
		setInteractive(true);
	}
		// TODO Figure out why copy doesn't quite work.
		// New button ends up being linked with the old button for events.
	Button::Button(const Button &rhs) :
		Sprite(rhs),
		mHitbox(rhs.mHitbox),
		mEnabled(rhs.mEnabled)
	{
		setHitbox(mHitbox);
		setInteractive(rhs.isInteractive());
	}
	Button::~Button()
	{
		if (mHitbox.get())
		{
			removeListeners(mHitbox);
		}
	}

	Renderable *Button::clone() const
	{
		return new Button(*this);
	}

	void Button::setHitbox(Renderable *hitbox)
	{
		if (mHitbox.get())
		{
			removeListeners(mHitbox);
		}
		else
		{
			removeListeners(this);
		}
		mHitbox = hitbox;
		if (mHitbox.get())
		{
			addListeners(mHitbox);
			mHitbox->setInteractive(true);
		}
		else
		{
			addListeners(this);
		}
	}
	Renderable *Button::getHitbox()
	{
		return mHitbox.get();
	}

	void Button::setEnabled(bool enabled)
	{
		mEnabled = enabled;
		if (enabled)
		{
			setSubWindowFrame(0);
		}
		else
		{
			setSubWindowFrame(3);
		}
	}
	bool Button::isEnabled() const
	{
		return mEnabled;
	}

	void Button::onEvent(ui::MouseEvent *e)
	{
		base::Handle<ui::Event> clickEvent;
		switch (e->getMouseEventType())
		{
		default:
		case ui::Mouse::MOUSE_OUT:
			setSubWindowFrame(0);
			break;
		case ui::Mouse::MOUSE_MOVE:
			{
				int frame = 1;
				if (ui::MouseManager::getManager()->getButtonDown(e->getMouseButton()))
				{
					frame = 2;
				}
				setSubWindowFrame(frame);
			}
			break;
		case ui::Mouse::MOUSE_UP:
			clickEvent = new ui::Event("click", this);
			fireEvent(clickEvent.get());
		case ui::Mouse::MOUSE_OVER:
			setSubWindowFrame(1);
			break;
		case ui::Mouse::MOUSE_DOWN:
			setSubWindowFrame(2);
			e->stopPropagation();
			break;
		}
	}

	void Button::addListeners(Renderable *target)
	{
		target->addEventListener(ui::Mouse::MOUSE_DOWN, this);
		target->addEventListener(ui::Mouse::MOUSE_MOVE, this);
		target->addEventListener(ui::Mouse::MOUSE_UP, this);
		target->addEventListener(ui::Mouse::MOUSE_OUT, this);
		target->addEventListener(ui::Mouse::MOUSE_OVER, this);
	}
	void Button::removeListeners(Renderable *target)
	{
		target->removeEventListener(ui::Mouse::MOUSE_DOWN, this);
		target->removeEventListener(ui::Mouse::MOUSE_MOVE, this);
		target->removeEventListener(ui::Mouse::MOUSE_UP, this);
		target->removeEventListener(ui::Mouse::MOUSE_OUT, this);
		target->removeEventListener(ui::Mouse::MOUSE_OVER, this);
	}

}
}
