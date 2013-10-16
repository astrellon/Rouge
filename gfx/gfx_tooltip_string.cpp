#include "gfx_tooltip_string.h"

#include <gfx/gfx_engine.h>
#include <gfx/gfx_texture.h>
#include <gl.h>

namespace am {
namespace gfx {

	TooltipString::TooltipString() :
		Tooltip(),
		mTextField(new TextField2()),
		mDetailedTextField(new TextField2())
	{
		addChild(mTextField);
		addChild(mDetailedTextField);

		mTextField->setPosition(8.0f, 8.0f);
		mDetailedTextField->setPosition(8.0f, 8.0f);

		mTooltipGraphic = mTextField;
	}
	TooltipString::TooltipString(const char *tooltip, const char *detailed, Renderable *target) :
		Tooltip(target),
		mTextField(new TextField2()),
		mDetailedTextField(new TextField2())
	{
		addChild(mTextField);
		addChild(mDetailedTextField);

		mTextField->setPosition(8.0f, 8.0f);
		mDetailedTextField->setPosition(8.0f, 8.0f);

		mTooltipGraphic = mTextField;

		setText(tooltip);
		setDetailedText(detailed);
	}
	TooltipString::TooltipString(const TooltipString &copy) :
		Tooltip(copy)
	{
		mTextField = dynamic_cast<TextField2 *>(mChildren[1].get());
		mDetailedTextField = dynamic_cast<TextField2 *>(mChildren[2].get());
	}
	TooltipString::~TooltipString()
	{
		
	}

	Renderable *TooltipString::clone() const
	{
		return new TooltipString(*this);
	}

	void TooltipString::setText(const char *text)
	{
		if (text == nullptr)
		{
			text = "";
		}
		float width = 0.0f, height = 0.0f;
		mTextField->setText(text);
		mTextField->getBaseFont()->measureText(mTextField->getText(), 0.0f, width, height);
		mTextField->setSize(width, height);
	}
	const char *TooltipString::getText() const
	{
		return mTextField->getText();
	}

	void TooltipString::setDetailedText(const char *text)
	{
		if (text == nullptr)
		{
			setTooltipDetailedGraphic(nullptr);
			text = "";
			return;
		}
		float width = 0.0f, height = 0.0f;
		mDetailedTextField->setText(text);
		mDetailedTextField->getBaseFont()->measureText(mDetailedTextField->getText(), 0.0f, width, height);
		mDetailedTextField->setSize(width, height);
		setTooltipDetailedGraphic(mDetailedTextField);
	}
	const char *TooltipString::getDetailedText() const
	{
		return mDetailedTextField->getText();
	}

	TextField2 *TooltipString::getTextField()
	{
		return mTextField;
	}
	TextField2 *TooltipString::getDetailedTextField()
	{
		return mDetailedTextField;
	}

}
}
