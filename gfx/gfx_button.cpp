#include "gfx_button.h"

#include "gfx_renderable.h"

#include <ui/mouse_manager.h>
using am::ui::MouseManager;

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
	Button::~Button()
	{
	}

	void Button::setHitbox(Renderable *hitbox)
	{
		if (mHitbox.get())
		{
			removeListeners(mHitbox.get());
		}
		else
		{
			removeListeners(this);
		}
		mHitbox = hitbox;
		if (mHitbox.get())
		{
			addListeners(mHitbox.get());
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
			setCurrentFrame(0);
		}
		else
		{
			setCurrentFrame(3);
		}
	}
	bool Button::isEnabled() const
	{
		return mEnabled;
	}

	void Button::onEvent(MouseEvent *e)
	{
		Handle<Event> clickEvent;
		switch (e->getMouseEventType())
		{
		default:
		case am::ui::MOUSE_OUT:
			setCurrentFrame(0);
			break;
		case am::ui::MOUSE_MOVE:
			{
				int frame = 1;
				if (MouseManager::getManager()->getButtonDown(e->getMouseButton()))
				{
					frame = 2;
				}
				setCurrentFrame(frame);
			}
			break;
		case am::ui::MOUSE_UP:
			clickEvent = new Event("click", this);
			fireEvent(clickEvent.get());
		case am::ui::MOUSE_OVER:
			setCurrentFrame(1);
			break;
		case am::ui::MOUSE_DOWN:
			setCurrentFrame(2);
			break;
		}
	}

	void Button::addListeners(Renderable *target)
	{
		target->addEventListener(MOUSE_DOWN, this);
		target->addEventListener(MOUSE_MOVE, this);
		target->addEventListener(MOUSE_UP, this);
		target->addEventListener(MOUSE_OUT, this);
		target->addEventListener(MOUSE_OVER, this);
	}
	void Button::removeListeners(Renderable *target)
	{
		target->removeEventListener(MOUSE_DOWN, this);
		target->removeEventListener(MOUSE_MOVE, this);
		target->removeEventListener(MOUSE_UP, this);
		target->removeEventListener(MOUSE_OUT, this);
		target->removeEventListener(MOUSE_OVER, this);
	}

}
}
