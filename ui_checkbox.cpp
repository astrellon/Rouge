#include "ui_checkbox.h"

#include "gfx/gfx_sprite.h"
#include "gfx/gfx_text_field.h"

#include "mouse_manager.h"

using namespace am::gfx;

namespace am {
namespace ui {

	Checkbox::Checkbox(GfxEngine *engine) :
		UIComponent(engine)
	{
		mCheckGraphic = new Sprite(engine);
		mLabel = new TextField(engine);
		init();
	}
	Checkbox::Checkbox(GfxEngine *engine, Asset *asset, const char *label) :
		UIComponent(engine)
	{
		mCheckGraphic = new Sprite(engine, asset);
		mLabel = new TextField(engine);
		mLabel->setText(label);
		init();
	}
	Checkbox::Checkbox(GfxEngine *engine, const char *assetName, const char *label) :
		UIComponent(engine)
	{
		mCheckGraphic = new Sprite(engine, assetName);
		mLabel = new TextField(engine);
		mLabel->setText(label);
		init();
	}
	Checkbox::~Checkbox()
	{
	}

	void Checkbox::setLabel(const char *label)
	{
		mLabel->setText(label);
	}
	void Checkbox::setLabel(const string &label)
	{
		mLabel->setText(label);
	}
	string Checkbox::getLabel() const
	{
		return mLabel->getText();
	}

	TextField *Checkbox::getLabelField()
	{
		return mLabel.get();
	}
	Sprite *Checkbox::getCheckGraphic()
	{
		return mCheckGraphic.get();
	}

	bool Checkbox::isSelected() const
	{
		return mSelected;
	}
	void Checkbox::setSelected(bool selected)
	{
		if (selected != mSelected)
		{
			mSelected = selected;
			JsonValue value(selected);
			DataEvent *e = new DataEvent("changed", this, selected);
			fireEvent(e);
			delete e;
		}
	}

	float Checkbox::getWidth()
	{
		return mCheckGraphic->getWidth() + mLabel->getMeasuredWidth();
	}
	float Checkbox::getHeight()
	{
		float heightCheck = mCheckGraphic->getHeight();
		float heightLabel = mLabel->getMeasuredHeight();
		return (heightCheck > heightLabel ? heightCheck : heightLabel);
	}

	void Checkbox::onEvent(MouseEvent *e)
	{
		int offset = mSelected ? 4 : 0;
		
		switch (e->getMouseEventType())
		{
		default:
		case am::ui::MOUSE_OUT:
			mCheckGraphic->setCurrentFrame(offset);
			break;
		case am::ui::MOUSE_MOVE:
			if (e->getManager()->getButtonDown(e->getMouseButton()))
			{
				mCheckGraphic->setCurrentFrame(offset + 2);
			}
			else
			{
				mCheckGraphic->setCurrentFrame(offset + 1);
			}
			break;
		case am::ui::MOUSE_UP:
			setSelected(!mSelected);
			offset = mSelected ? 4 : 0;
		case am::ui::MOUSE_OVER:
			mCheckGraphic->setCurrentFrame(offset + 1);
			break;
		case am::ui::MOUSE_DOWN:
			mCheckGraphic->setCurrentFrame(offset + 2);
			break;
		}
	}

	void Checkbox::init()
	{
		mSelected = false;
		addChild(mCheckGraphic.get());
/*		mCheckGraphic->setNumFramesX(2);
		mCheckGraphic->setNumFramesY(4);
		mCheckGraphic->setNumTotalFrames(8);*/
		
		addChild(mLabel.get());
		mLabel->setBaseFont("arial");
		float labelTop = (getHeight() - mLabel->getMeasuredHeight()) * 0.5f;
		mLabel->setPosition(mCheckGraphic->getWidth(), labelTop + 1);

		mHitbox = new Sprite(mGfxEngine);
		mHitbox->getColour().setAlpha(0.0f);
		addChild(mHitbox.get());

		mHitbox->addEventListener("mouse_over", this);
		mHitbox->addEventListener("mouse_out", this);
		mHitbox->addEventListener("mouse_down", this);
		mHitbox->addEventListener("mouse_up", this);
		mHitbox->addEventListener("mouse_move", this);

		mHitbox->setWidth(getWidth());
		mHitbox->setHeight(getHeight());
		mHitbox->setInteractive(true);

	}

	void Checkbox::setEnabled(bool enabled)
	{
		UIComponent::setEnabled(enabled);
		int offset = mSelected ? 4 : 0;
		if (enabled)
		{
			mCheckGraphic->setCurrentFrame(offset);
		}
		else
		{
			mCheckGraphic->setCurrentFrame(offset + 3);
		}
	}
}
}
