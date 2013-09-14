#include "gfx_tooltip_component.h"

#include <gfx/gfx_renderable.h>

namespace am {
namespace gfx {

	TooltipComponent::TooltipComponent() :
		mParent(nullptr)
	{
	}
	TooltipComponent::TooltipComponent(const TooltipComponent &copy) :
		mTooltip(copy.mTooltip),
		mDetailedTooltip(copy.mDetailedTooltip),
		mParent(nullptr)
	{

	}
	TooltipComponent::~TooltipComponent()
	{
	}

	void TooltipComponent::setTooltip(const char *tooltip)
	{
		if (!tooltip)
		{
			mTooltip.clear();
			return;
		}
		mTooltip = tooltip;
	}
	const char *TooltipComponent::getTooltip() const
	{
		return mTooltip.c_str();
	}

	void TooltipComponent::setDetailedTooltip(const char *tooltip)
	{
		if (!tooltip)
		{
			mDetailedTooltip.clear();
			return;
		}
		mDetailedTooltip = tooltip;
	}
	const char *TooltipComponent::getDetailedTooltip() const
	{
		return mDetailedTooltip.c_str();
	}

	void TooltipComponent::setParent(Renderable *parent)
	{
		if (mParent == parent)
		{
			return;
		}
		if (mParent)
		{
			mParent->release();
		}
		mParent = parent;
		if (mParent)
		{
			mParent->retain();
		}
	}
	Renderable *TooltipComponent::getParent() const
	{
		return mParent;
	}

}
}
