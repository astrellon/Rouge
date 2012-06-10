#include "ui_options_panel.h"

#include "gfx/gfx_engine.h"

#include "ui_button.h"
#include "ui_checkbox.h"
#include "ui_image.h"

#include "colour.h"

namespace am {
namespace ui {

	OptionsPanel::OptionsPanel(GfxEngine *engine) :
		UIComponent(engine),
		mFullscreen(new Checkbox(engine, "checkbox", "Fullscreen")),
		mApply(new Button(engine, "bigButton", "Apply")),
		mClose(new Button(engine, "bigButton", "Close")),
		mBackground(new Image(engine, "bigButton"))
	{
		addChild(mBackground.get());
		mBackground->setAlpha(0.5f);
		addChild(mFullscreen.get());
		addChild(mApply.get());
		addChild(mClose.get());

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
			Event *e = new Event("close_options", this);
			fireEvent(e);
			delete e;
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
