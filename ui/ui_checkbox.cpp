#include "ui_checkbox.h"

#include <gfx/gfx_sprite.h>
#include <gfx/gfx_text_field.h>

#include "mouse_manager.h"

namespace am {
namespace ui {

	Checkbox::Checkbox() :
		UIComponent()
	{
		mCheckGraphic = new gfx::Sprite();
		mLabel = new gfx::TextField();
		init();
	}
	Checkbox::Checkbox(gfx::Asset *asset, const char *label) :
		UIComponent()
	{
		mCheckGraphic = new gfx::Sprite(asset);
		mLabel = new gfx::TextField();
		mLabel->setText(label);
		init();
	}
	Checkbox::Checkbox(const char *assetName, const char *label) :
		UIComponent()
	{
		mCheckGraphic = new gfx::Sprite(assetName);
		mLabel = new gfx::TextField();
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
	void Checkbox::setLabel(const std::string &label)
	{
		mLabel->setText(label);
	}
	std::string Checkbox::getLabel() const
	{
		return mLabel->getText();
	}

	gfx::TextField *Checkbox::getLabelField()
	{
		return mLabel.get();
	}
	gfx::Sprite *Checkbox::getCheckGraphic()
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
			base::Handle<ValueChangeEvent> e(new ValueChangeEvent(this, selected));
			fireEvent<ValueChangeEvent>(e);
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
		case ui::Mouse::MOUSE_OUT:
			mCheckGraphic->setCurrentFrame(offset);
			break;
		case ui::Mouse::MOUSE_MOVE:
			if (MouseManager::getManager()->getButtonDown(e->getMouseButton()))
			{
				mCheckGraphic->setCurrentFrame(offset + 2);
			}
			else
			{
				mCheckGraphic->setCurrentFrame(offset + 1);
			}
			break;
		case ui::Mouse::MOUSE_UP:
			setSelected(!mSelected);
			offset = mSelected ? 4 : 0;
		case ui::Mouse::MOUSE_OVER:
			mCheckGraphic->setCurrentFrame(offset + 1);
			break;
		case ui::Mouse::MOUSE_DOWN:
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
		mLabel->setBaseFont("default:arial");
		float labelTop = (getHeight() - mLabel->getMeasuredHeight()) * 0.5f;
		mLabel->setPosition(mCheckGraphic->getWidth(), labelTop + 1);

		mHitbox = new gfx::Sprite();
		mHitbox->setGfxComponent(new gfx::GfxComponent());
		mHitbox->getGfxComponent()->getColour().setAlpha(0.0f);
		addChild(mHitbox.get());

		mHitbox->addEventListener(ui::Mouse::MOUSE_OVER, this);
		mHitbox->addEventListener(ui::Mouse::MOUSE_OUT, this);
		mHitbox->addEventListener(ui::Mouse::MOUSE_DOWN, this);
		mHitbox->addEventListener(ui::Mouse::MOUSE_UP, this);
		mHitbox->addEventListener(ui::Mouse::MOUSE_MOVE, this);

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
