#include "ui_component.h"

#include "gfx/gfx_engine.h"

namespace am {
namespace ui {

	UIComponent::UIComponent(GfxEngine *engine) :
		Layer(engine),
		mAnchorX(X_LEFT),
		mAnchorY(Y_TOP),
		mOffsetX(0.0f),
		mOffsetY(0.0f)
	{
	}
	UIComponent::~UIComponent()
	{
	}

	void UIComponent::setAnchorX(AnchorX anchor)
	{
		mAnchorX = anchor;
	}
	void UIComponent::setAnchorY(AnchorY anchor)
	{
		mAnchorY = anchor;
	}
	void UIComponent::setAnchor(AnchorX anchorX, AnchorY anchorY)
	{
		mAnchorX = anchorX;
		mAnchorY = anchorY;
	}

	AnchorX UIComponent::getAnchorX() const
	{
		return mAnchorX;
	}
	AnchorY UIComponent::getAnchorY() const
	{
		return mAnchorY;
	}

	void UIComponent::setOffsetX(float offset)
	{
		mOffsetX = offset;
	}
	void UIComponent::setOffsetY(float offset)
	{
		mOffsetY = offset;
	}
	void UIComponent::setOffset(float offsetX, float offsetY)
	{
		mOffsetX = offsetX;
		mOffsetY = offsetY;
	}

	float UIComponent::getOffsetX() const
	{
		return mOffsetX;
	}
	float UIComponent::getOffsetY() const
	{
		return mOffsetY;
	}

	float UIComponent::getParentWidth()
	{
		if (mParent != NULL)
		{
			return mParent->getWidth();
		}
		return static_cast<float>(mGfxEngine->getScreenWidth());
	}
	float UIComponent::getParentHeight()
	{
		if (mParent != NULL)
		{
			return mParent->getHeight();
		}
		return static_cast<float>(mGfxEngine->getScreenHeight());
	}

	void UIComponent::preRender(float dt)
	{
		float left = 0.0f;
		if (mAnchorX == X_CENTER)
		{
			left = (getParentWidth() - getWidth()) * 0.5f;
		}
		else if (mAnchorX == X_RIGHT)
		{
			left = getParentWidth() - getWidth();
		}
		left += mOffsetX;

		float top = 0.0f;
		if (mAnchorY == Y_CENTER)
		{
			top = (getParentHeight() - getHeight()) * 0.5f;
		}
		else if (mAnchorY == Y_BOTTOM)
		{
			top = getParentHeight() - getHeight();
		}
		top += mOffsetY;

		setPosition(left, top);

		Layer::preRender(dt);
	}

}
}
