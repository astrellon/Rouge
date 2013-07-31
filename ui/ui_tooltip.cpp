#include "ui_tooltip.h"

#include <gfx/gfx_engine.h>

namespace am {
namespace ui {

	Tooltip::Tooltip() :
		mGraphic(new Sprite("tooltip")),
		mTextField(new TextField2()),
		mDetailedTextField(new TextField2()),
		mState(HIDDEN),
		mTimeCounter(0.0f),
		mDisplayDelay(0.8f),
		mDetailedDisplayDelay(1.5f)
	{
		addChild(mGraphic);
		addChild(mTextField);
		addChild(mDetailedTextField);

		mTextField->setPosition(8.0f, 8.0f);
		mDetailedTextField->setPosition(8.0f, 8.0f);
	}
	Tooltip::Tooltip(const char *tooltip, const char *detailed, Renderable *target) :
		mGraphic(new Sprite("tooltip")),
		mTextField(new TextField2()),
		mDetailedTextField(new TextField2()),
		mState(HIDDEN),
		mTimeCounter(0.0f),
		mDisplayDelay(0.8f),
		mDetailedDisplayDelay(1.5f),
		mTarget(target)
	{
		addChild(mGraphic);
		addChild(mTextField);
		addChild(mDetailedTextField);

		mTextField->setPosition(8.0f, 8.0f);
		mDetailedTextField->setPosition(8.0f, 8.0f);

		setText(tooltip);
		setDetailedText(detailed);
	}
	Tooltip::~Tooltip()
	{
		
	}

	void Tooltip::setText(const char *text)
	{
		if (text == nullptr)
		{
			text = "";
		}
		mTextWidth = mTextHeight = 0.0f;
		mTextField->setText(text);
		mTextField->getBaseFont()->measureText(mTextField->getText(), 0.0f, mTextWidth, mTextHeight);
	}
	const char *Tooltip::getText() const
	{
		return mTextField->getText();
	}

	void Tooltip::setDetailedText(const char *text)
	{
		if (text == nullptr)
		{
			text = "";
		}
		mDetailedTextWidth = mDetailedTextHeight = 0.0f;
		mDetailedTextField->setText(text);
		mDetailedTextField->getBaseFont()->measureText(mDetailedTextField->getText(), 0.0f, mDetailedTextWidth, mDetailedTextHeight);
	}
	const char *Tooltip::getDetailedText() const
	{
		return mDetailedTextField->getText();
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

	Sprite *Tooltip::getGraphic()
	{
		return mGraphic;
	}
	TextField2 *Tooltip::getTextField()
	{
		return mTextField;
	}
	TextField2 *Tooltip::getDetailedTextField()
	{
		return mDetailedTextField;
	}

	void Tooltip::setDetailed(bool detailed)
	{
		if (detailed)
		{
			mTextField->setVisible(false);
			mDetailedTextField->setVisible(true);

			mGraphic->setSize(mDetailedTextWidth + 16.0f, mDetailedTextHeight + 16.0f);
		}
		else
		{
			mTextField->setVisible(true);
			mDetailedTextField->setVisible(false);

			mGraphic->setSize(mTextWidth + 16.0f, mTextHeight + 16.0f);
		}
	}

	void Tooltip::render(float dt)
	{
		if (mState != HIDDEN)
		{
			mTimeCounter += dt;
			if (mState == ACTIVE)
			{
				if (mTimeCounter > mDisplayDelay)
				{
					setState(VISIBLE);
					show(mTarget);
				}
			}
			else if (mState == VISIBLE)
			{
				if (mTimeCounter > mDetailedDisplayDelay && mDetailedTextField->rawLength() > 0)
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

}
}
