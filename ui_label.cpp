#include "ui_label.h"

#include "gfx/gfx_engine.h"
#include "gfx/gfx_text_field.h"

namespace am {
namespace ui {

	Label::Label(GfxEngine *engine) :
		UIComponent(engine)
	{
		mLabel = new TextField(engine);
		addChild(mLabel);
	}
	Label::Label(GfxEngine *engine, const char *label) :
		UIComponent(engine)
	{
		mLabel = new TextField(engine);
		mLabel->setText(label);
		addChild(mLabel);
	}
	Label::Label(GfxEngine *engine, const string &label) :
		UIComponent(engine)
	{
		mLabel = new TextField(engine);
		mLabel->setText(label);
		addChild(mLabel);
	}
	Label::~Label()
	{
	}

	void Label::setLabel(const char *label)
	{
		mLabel->setText(label);
	}
	void Label::setLabel(const string &label)
	{
		mLabel->setText(label);
	}
	string Label::getLabel() const
	{
		return mLabel->getText();
	}

	TextField *Label::getLabelField()
	{
		return mLabel;
	}

	float Label::getWidth()
	{
		return mLabel->getMeasuredWidth();
	}
	float Label::getHeight()
	{
		return mLabel->getMeasuredHeight();
	}

}
}
