#include "gfx_tooltip.h"

#include <gfx/gfx_engine.h>
#include <gfx/gfx_texture.h>
#include <gl.h>

#include <ui/mouse_manager.h>

namespace am {
namespace gfx {

	Tooltip::Tooltip() :
		Layer(),
		mGraphic(new Sprite("ui:tooltip")),
		mState(HIDDEN),
		mTimeCounter(0.0f),
		mDisplayDelay(0.8f),
		mDetailedDisplayDelay(1.5f)
	{
		addChild(mGraphic);
	}
	Tooltip::Tooltip(Renderable *target) :
		Layer(),
		mGraphic(new Sprite("ui:tooltip")),
		mState(HIDDEN),
		mTimeCounter(0.0f),
		mDisplayDelay(0.8f),
		mDetailedDisplayDelay(1.5f),
		mTarget(target)
	{
		addChild(mGraphic);
	}
	Tooltip::Tooltip(const Tooltip &copy) :
		Layer(copy),
		mState(HIDDEN),
		mTimeCounter(0.0f),
		mDisplayDelay(copy.mDisplayDelay),
		mDetailedDisplayDelay(copy.mDetailedDisplayDelay),
		mTarget(nullptr)
	{
		if (copy.mGraphic)
		{
			for (auto iter = mChildren.begin(); iter != mChildren.end(); ++iter)
			{
				Sprite *sprite = dynamic_cast<Sprite *>(iter->get());
				if (sprite && sprite->getAsset() == copy.mGraphic->getAsset())
				{
					mGraphic = sprite;
					break;
				}
			}
		}
	}
	Tooltip::~Tooltip()
	{
		
	}

	Renderable *Tooltip::clone() const
	{
		return new Tooltip(*this);
	}

	void Tooltip::active(Renderable *target)
	{
		setState(ACTIVE);
		GfxEngine::getEngine()->getTooltipLayer()->addChild(this);
		if (target)
		{
			setTarget(target);
		}
	}
	void Tooltip::show(Renderable *target)
	{
		setState(VISIBLE);
		setDetailed(false);
		GfxEngine::getEngine()->getTooltipLayer()->addChild(this);
		if (target)
		{
			setTarget(target);
		}
	}
	void Tooltip::hide()
	{
		GfxEngine::getEngine()->getTooltipLayer()->removeChild(this);
		setState(HIDDEN);
	}

	void Tooltip::setTarget(Renderable *target)
	{
		mTarget = target;

		if (target != nullptr)
		{
			float outX = 0.0f;
			float outY = 0.0f;
			target->getLocalToScreen(0, 0, outX, outY);
			setPosition(outX, outY);
		}
	}
	Renderable *Tooltip::getTarget() const
	{
		return mTarget;
	}

	void Tooltip::setState(TooltipState state)
	{
		mState = state;
		mTimeCounter = 0.0f;
	}
	Tooltip::TooltipState Tooltip::getState() const
	{
		return mState;
	}

	void Tooltip::setDisplayDelay(float delay)
	{
		mDisplayDelay = delay;
	}
	float Tooltip::getDisplayDelay() const
	{
		return mDisplayDelay;
	}

	void Tooltip::setDetailedDisplayDelay(float delay)
	{
		mDetailedDisplayDelay = delay;
	}
	float Tooltip::getDetailedDisplayDelay() const
	{
		return mDetailedDisplayDelay;
	}

	void Tooltip::setDelays(float displayDelay, float detailedDelay)
	{
		setDisplayDelay(displayDelay);
		setDetailedDisplayDelay(detailedDelay);
	}

	float Tooltip::getTimeCounter() const
	{
		return mTimeCounter;
	}

	Sprite *Tooltip::getGraphic() const
	{
		return mGraphic;
	}
	void Tooltip::setTooltipGraphic(Renderable *tooltip)
	{
		mTooltipGraphic = tooltip;
	}
	Renderable *Tooltip::getTooltipGraphic() const
	{
		return mTooltipGraphic;
	}
	void Tooltip::setTooltipDetailedGraphic(Renderable *tooltip)
	{
		mTooltipDetailedGraphic = tooltip;
	}
	Renderable *Tooltip::getTooltipDetailedGraphic() const
	{
		return mTooltipDetailedGraphic;
	}

	void Tooltip::setDetailed(bool detailed)
	{
		if (detailed)
		{
			if (mTooltipGraphic)
			{
				mTooltipGraphic->setVisible(false);
			}
			if (mTooltipDetailedGraphic)
			{
				mTooltipDetailedGraphic->setVisible(true);
				mGraphic->setSize(mTooltipDetailedGraphic->getWidth() + 16.0f, mTooltipDetailedGraphic->getHeight() + 16.0f);
			}
		}
		else
		{
			if (mTooltipGraphic)
			{
				mTooltipGraphic->setVisible(true);
				mGraphic->setSize(mTooltipGraphic->getWidth() + 16.0f, mTooltipGraphic->getHeight() + 16.0f);
			}
			if (mTooltipDetailedGraphic)
			{
				mTooltipDetailedGraphic->setVisible(false);
			}
		}
	}

	void Tooltip::render(float dt)
	{
		if (mState != HIDDEN)
		{
			mTimeCounter += dt;
			if (mState == ACTIVE)
			{
				if (mTimeCounter > mDetailedDisplayDelay && mTooltipDetailedGraphic)
				{
					setState(VISIBLE_DETAILED);
					setDetailed(true);
				}
				else if (mTimeCounter > mDisplayDelay)
				{
					setState(VISIBLE);
					show(mTarget);
				}
			}
			else if (mState == VISIBLE)
			{
				if (mTimeCounter > mDetailedDisplayDelay && mTooltipDetailedGraphic)
				{
					setState(VISIBLE_DETAILED);
					setDetailed(true);
				}
			}
		}
		if (mState == HIDDEN || mState == ACTIVE)
		{
			return;
		}

		Layer::render(dt);
	}

	void Tooltip::onEvent(MouseEvent *e)
	{
		if (!e || !mTarget)
		{
			return;
		}

		if (e->getMouseEventType() == MOUSE_OVER)
		{
			MouseManager *manager = MouseManager::getManager();
			setPosition(manager->getMouseX(), manager->getMouseY());
			active();
		}
		else
		{
			hide();
		}
	}

}
}
