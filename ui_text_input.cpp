#include "ui_text_input.h"

namespace am {
namespace ui {

	TextInput::TextInput() :
		UIComponent(),
		mInputPosition(0),
		mText(new TextField())
	{
		addChild(mText.get());
	}
	TextInput::~TextInput()
	{

	}

	void TextInput::setText(const char *text)
	{
		setText(string(text));
	}
	void TextInput::setText(const string &text)
	{
		mText->setText(text);
		mInputPosition = text.length();
	}
	string TextInput::getText() const
	{
		return mText->getText();
	}

	void TextInput::appendText(char text)
	{
		static char buff[2];
		buff[0] = text;
		buff[1] = '\0';
		string str = getText();
		str.insert(mInputPosition, buff);
		mText->setText(str);
		mInputPosition++;
	}
	void TextInput::appendText(const char *text)
	{

	}
	void TextInput::appendText(const string &text)
	{

	}

	int TextInput::getInputPosition() const
	{
		return mInputPosition;
	}
	void TextInput::moveInputPosition(int move)
	{
		setInputPosition(mInputPosition + move);
	}
	void TextInput::setInputPosition(int pos)
	{
		pos = pos < 0 ? 0 : pos;
		int maxPos = static_cast<int>(getText().length());
		pos = pos > maxPos ? maxPos : pos;
		mInputPosition = pos;
	}

	TextField *TextInput::getTextField()
	{
		return mText.get();
	}

	float TextInput::getWidth()
	{
		return mText->getWidth();
	}
	float TextInput::getHeight()
	{
		return mText->getHeight();
	}

}
}
