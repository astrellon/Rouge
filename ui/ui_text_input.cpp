#include "ui_text_input.h"

#include <ui/keyboard_manager.h>

#include <util/utils.h>
using namespace am::util;

namespace am {
namespace ui {

	TextInput::TextInput() :
		UIComponent(),
		mFocus(false),
		mInputPosition(0),
		mText(new TextField())
	{
		addChild(mText.get());
		mText->setCursorInputPosition(0);
	}
	TextInput::~TextInput()
	{

	}

	void TextInput::onEvent(KeyboardEvent *e)
	{
		if (!e)
		{
			return;
		}
		if (e->isSystemKey())
		{
			int key = e->getKey();
			bool ctrlDown = KeyboardManager::getManager()->isKeyDown(17);
			// Home key
			if (key == 36)
			{
				// Go to very start
				if (ctrlDown)
				{
					setInputPosition(0);
				}
				else
				{
					int position = mText->getStartOfLine(mInputPosition);
					setInputPosition(position);
				}
			}
			// End key
			else if (key == 35)
			{
				// Go to very end
				if (ctrlDown)
				{
					setInputPosition(mText->length());
				}
				else
				{
					int position = mText->getEndOfLine(mInputPosition);
					setInputPosition(position);
				}
			}
			// Left key
			else if (key == 37)
			{
				if (ctrlDown)
				{
					int count = Utils::countWordLetters(getText(), mInputPosition, false);
					if (count != 0)
					{
						updateInputCursor(count);
						return;
					}
				}
				updateInputCursor(-1);
			}
			// Right key
			else if(key == 39)
			{
				if (ctrlDown)
				{
					int count = Utils::countWordLetters(getText(), mInputPosition, true);
					if (count != 0)
					{
						updateInputCursor(count);
						return;
					}
				}
				updateInputCursor(1);
			}
		}
		else
		{
			appendText(e->getKey());
		}
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
		if (text == 8)
		{
			string str = getText();
			if (str.size() > 0 && mInputPosition > 0)
			{
				str.erase(mInputPosition - 1, 1);
				mText->setText(str);
				//mInputPosition--;
				updateInputCursor(-1);
			}
			return;
		}
		else if (text == 13)
		{
			text = '\n';
		}
		else if (text < ' ' && text != '\t')
		{
			return;
		}
		static char buff[2];
		buff[0] = text;
		buff[1] = '\0';
		string str = getText();
		str.insert(mInputPosition, buff);
		mText->setText(str);
		//mInputPosition++;
		updateInputCursor(1);
	}
	void TextInput::appendText(const char *text)
	{
		string str = getText();
		str.insert(mInputPosition, text);
		mText->setText(str);
		//mInputPosition += ;
		updateInputCursor(strlen(text));
	}
	void TextInput::appendText(const string &text)
	{
		string str = getText();
		str.insert(mInputPosition, text);
		mText->setText(str);
		//mInputPosition += text.size();
		updateInputCursor(text.size());
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
		mText->setCursorInputPosition(pos);
	}

	TextField *TextInput::getTextField()
	{
		return mText.get();
	}

	void TextInput::setFocus(bool focus)
	{
		if (focus != mFocus)
		{
			if (!focus)
			{
				KeyboardManager::getManager()->removeEventListener(KEY_DOWN, this);
			}
			else
			{
				KeyboardManager::getManager()->addEventListener(KEY_DOWN, this);
			}
			mFocus = focus;
		}
	}

	float TextInput::getWidth()
	{
		return mText->getWidth();
	}
	float TextInput::getHeight()
	{
		return mText->getHeight();
	}

	void TextInput::updateInputCursor(int diff)
	{
		mInputPosition += diff;
		if (mInputPosition < 0)
		{
			mInputPosition = 0;
		}
		if (mInputPosition >= mText->length())
		{
			mInputPosition = mText->length();
		}
		mText->setCursorInputPosition(mInputPosition);
	}
}
}
