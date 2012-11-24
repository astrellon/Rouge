#include "ui_options_panel.h"

#include <sys/rouge_system.h>
#include <log/logger.h>

#include <util/colour.h>

#include "ui_text_button.h"
#include "ui_checkbox.h"
#include "ui_image.h"

namespace am {
namespace ui {

	OptionsPanel::OptionsPanel(RougeSystem *system) :
		UIComponent(),
		mRougeSystem(system),
		mFullscreen(new Checkbox("checkbox", "Fullscreen")),
		mApply(new TextButton("bigButton", "Apply")),
		mClose(new TextButton("bigButton", "Close")),
		mBackground(new Image("bigButton"))
	{
		addChild(mBackground);
		mBackground->setGfxComponent(new GfxComponent());
		mBackground->getGfxComponent()->setAlpha(0.5f);
		addChild(mFullscreen.get());
		addChild(mApply.get());
		addChild(mClose.get());

		mFullscreen->setParentOffset(20.0f, 80.0f);
		mFullscreen->addEventListener("change", this);

		mApply->setWidth(100.0f);
		mApply->setParentAnchor(X_RIGHT, Y_BOTTOM);
		mApply->setAnchor(X_RIGHT, Y_BOTTOM);
		mApply->setParentOffset(-20.0f, -20.0f);
		mApply->addEventListener("click", this);

		mClose->setWidth(100.0f);
		mClose->setParentAnchor(X_RIGHT, Y_BOTTOM);
		mClose->setAnchor(X_RIGHT, Y_BOTTOM);
		mClose->setParentOffset(-120.0f, -20.0f);
		mClose->addEventListener("click", this);
	}

	OptionsPanel::~OptionsPanel()
	{
	}

	void OptionsPanel::onEvent(Event *e)
	{
		if (e->getEventTarget() == mApply.get() || 
			e->getEventTarget() == mClose.get())
		{
			mRougeSystem->closeOptionsPanel();
		}
	}
	void OptionsPanel::onEvent(ValueChangeEvent *e)
	{
		if (e->getEventTarget() == mFullscreen)
		{
			mRougeSystem->setFullscreen(mFullscreen->isSelected());
		}
	}

	void OptionsPanel::setWidth(float width)
	{
		mBackground->setWidth(width);
		UIComponent::setWidth(width);
	}
	void OptionsPanel::setHeight(float height)
	{
		mBackground->setHeight(height);
		UIComponent::setHeight(height);
	}
}
}
