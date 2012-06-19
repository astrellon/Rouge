#include "ui_component.h"

#include "gfx/gfx_engine.h"

namespace am {
namespace ui {

	UIComponent::UIComponent() :
		Layer(),
		mAnchorX(X_LEFT),
		mAnchorY(Y_TOP),
		mOffsetX(0.0f),
		mOffsetY(0.0f),
		mParentAnchorX(X_LEFT),
		mParentAnchorY(Y_TOP),
		mParentOffsetX(0.0f),
		mParentOffsetY(0.0f),
		mEnabled(true)
	{
		setInteractive(true);
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

	void UIComponent::setParentAnchorX(AnchorX anchor)
	{
		mParentAnchorX = anchor;
	}
	void UIComponent::setParentAnchorY(AnchorY anchor)
	{
		mParentAnchorY = anchor;
	}
	void UIComponent::setParentAnchor(AnchorX anchorX, AnchorY anchorY)
	{
		mParentAnchorX = anchorX;
		mParentAnchorY = anchorY;
	}

	AnchorX UIComponent::getParentAnchorX() const
	{
		return mParentAnchorX;
	}
	AnchorY UIComponent::getParentAnchorY() const
	{
		return mParentAnchorY;
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

	void UIComponent::setParentOffsetX(float offset)
	{
		mParentOffsetX = offset;
	}
	void UIComponent::setParentOffsetY(float offset)
	{
		mParentOffsetY = offset;
	}
	void UIComponent::setParentOffset(float offsetX, float offsetY)
	{
		mParentOffsetX = offsetX;
		mParentOffsetY = offsetY;
	}

	float UIComponent::getParentOffsetX() const
	{
		return mParentOffsetX;
	}
	float UIComponent::getParentOffsetY() const
	{
		return mParentOffsetY;
	}

	float UIComponent::getParentWidth()
	{
		if (mParent)
		{
			return mParent->getWidth();
		}
		return static_cast<float>(GfxEngine::getGfxEngine()->getScreenWidth());
	}
	float UIComponent::getParentHeight()
	{
		if (mParent)
		{
			return mParent->getHeight();
		}
		return static_cast<float>(GfxEngine::getGfxEngine()->getScreenHeight());
	}

	void UIComponent::preRender(float dt)
	{
		float parentLeft = 0.0f;
		if (mParentAnchorX == X_CENTER)
		{
			parentLeft = getParentWidth() * 0.5f;
		}
		else if(mParentAnchorX == X_RIGHT)
		{
			parentLeft = getParentWidth();
		}
		parentLeft += mParentOffsetX;

		float parentTop = 0.0f;
		if (mParentAnchorY == Y_CENTER)
		{
			parentTop = getParentHeight() * 0.5f;
		}
		else if(mParentAnchorY == Y_BOTTOM)
		{
			parentTop = getParentHeight();
		}
		parentTop += mParentOffsetY;

		float left = 0.0f;
		if (mAnchorX == X_CENTER)
		{
			left = getWidth() * 0.5f;
		}
		else if(mAnchorX == X_RIGHT)
		{
			left = getWidth();
		}
		left += mOffsetX;

		float top = 0.0f;
		if (mAnchorY == Y_CENTER)
		{
			top = getHeight() * 0.5f;
		}
		else if(mAnchorY == Y_BOTTOM)
		{
			top = getHeight();
		}
		top += mOffsetY;
		setPosition(parentLeft - left, parentTop - top);

		Layer::preRender(dt);
	}

	bool UIComponent::isInteractive() const
	{
		return mEnabled && Layer::isInteractive();
	}

	void UIComponent::setEnabled(bool enabled)
	{
		mEnabled = enabled;
	}
	bool UIComponent::isEnabled() const
	{
		return mEnabled;
	}

}
}
