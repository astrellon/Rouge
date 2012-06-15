#include "ui_label.h"

#include "gfx/gfx_text_field.h"

namespace am {
namespace ui {

	Label::Label() :
		UIComponent()
	{
		mLabel = new TextField();
		addChild(mLabel.get());
	}
	Label::Label(const char *label) :
		UIComponent()
	{
		mLabel = new TextField();
		mLabel->setText(label);
		addChild(mLabel.get());
	}
	Label::Label(const string &label) :
		UIComponent()
	{
		mLabel = new TextField();
		mLabel->setText(label);
		addChild(mLabel.get());
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
		return mLabel.get();
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
