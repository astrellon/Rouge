#include "gfx_tooltip_component.h"

#include <gfx/gfx_renderable.h>

#include <ui/ui_tooltip.h>
#include <ui/mouse_manager.h>
using namespace am::ui;

#include <sys/game_system.h>
using namespace am::sys;

namespace am {
namespace gfx {

	TooltipComponent::TooltipComponent() :
		mParent(nullptr),
		mListeners(false),
		mEnabled(true)
	{
	}
	TooltipComponent::TooltipComponent(Renderable *parent) :
		mParent(parent),
		mListeners(false),
		mEnabled(true)
	{

	}
	TooltipComponent::TooltipComponent(const TooltipComponent &copy) :
		mTooltip(copy.mTooltip),
		mDetailedTooltip(copy.mDetailedTooltip),
		mParent(nullptr),
		mListeners(false),
		mEnabled(copy.mEnabled)
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
			updateListeners();
			return;
		}
		mTooltip = tooltip;
		updateListeners();
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
			updateListeners();
			return;
		}
		mDetailedTooltip = tooltip;
		updateListeners();
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
			removeListeners();
		}
		mParent = parent;
		if (mParent)
		{
			mParent->retain();
		}
		updateListeners();
	}
	Renderable *TooltipComponent::getParent() const
	{
		return mParent;
	}

	void TooltipComponent::setEnabled(bool enabled)
	{
		mEnabled = enabled;
		if (!enabled)
		{
			Tooltip *tooltip = getTooltipObject();
			tooltip->hide();
		}
	}
	bool TooltipComponent::isEnabled() const
	{
		return mEnabled;
	}

	void TooltipComponent::onEvent(MouseEvent *e)
	{
		if (!e)
		{
			return;
		}

		Tooltip *tooltip = getTooltipObject();
		if (e->getMouseEventType() == MOUSE_OVER && mEnabled)
		{
			tooltip->setText(mTooltip.c_str());
			tooltip->setDetailedText(mDetailedTooltip.c_str());
			MouseManager *manager = MouseManager::getManager();
			tooltip->setPosition(manager->getMouseX(), manager->getMouseY());
			tooltip->active();
		}
		else
		{
			tooltip->hide();
		}
	}

	void TooltipComponent::updateListeners()
	{
		if (mListeners)
		{
			if (mTooltip.empty() && mDetailedTooltip.empty() && mParent)
			{
				removeListeners();
			}
		}
		else
		{
			if (!mTooltip.empty() || !mDetailedTooltip.empty() && mParent)
			{
				addListeners();
			}
		}
	}
	void TooltipComponent::addListeners()
	{
		mListeners = true;
		mParent->addEventListener(MOUSE_OVER, this);
		mParent->addEventListener(MOUSE_OUT, this);
	}
	void TooltipComponent::removeListeners()
	{
		mListeners = false;
		mParent->removeEventListener(MOUSE_OVER, this);
		mParent->removeEventListener(MOUSE_OUT, this);
	}

	Tooltip *TooltipComponent::getTooltipObject() const
	{
		return GameSystem::getGameSystem()->getDefaultTooltip();
	}

}
}
