#include "gfx_tooltip_component.h"

namespace am {
namespace gfx {

	TooltipComponent::TooltipComponent()
	{
	}
	TooltipComponent::TooltipComponent(const TooltipComponent &copy) :
		mTooltip(copy.mTooltip),
		mDetailedTooltip(copy.mDetailedTooltip)
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

}
}
