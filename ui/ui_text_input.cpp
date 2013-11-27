#include "ui_text_input.h"

#include <ui/keyboard_manager.h>
#include <ui/mouse_manager.h>

#include <log/logger.h>

#include <util/utils.h>
using namespace am::util;

#include <gfx/gfx_texture.h>

#include <gl.h>

namespace am {
namespace ui {

	TextInput::TextInput() :
		UIComponent(),
		mFocus(false),
		mInputPosition(0),
		mText(new TextField()),
		mMaxCharacters(0),
		mRestriction(NONE)
	{
		addChild(mText);
		mText->setCursorInputPosition(0);
		mText->setInteractive(true);
		mText->setBaseFont("default:arial");
		setName("Text Input");

		mText->addEventListener(Mouse::MOUSE_DOWN, this);
		MouseManager::getManager()->addEventListener(Mouse::MOUSE_DOWN, this);
	}
	TextInput::TextInput(const TextInput &copy) :
		UIComponent(copy),
		mFocus(false),
		mInputPosition(0),
		mMaxCharacters(copy.mMaxCharacters),
		mRestriction(copy.mRestriction)
	{
		mText = dynamic_cast<TextField *>(mChildren[0].get());
		mText->addEventListener(Mouse::MOUSE_DOWN, this);
		MouseManager::getManager()->addEventListener(Mouse::MOUSE_DOWN, this);
	}
	TextInput::~TextInput()
	{
		mText->removeEventListener(Mouse::MOUSE_DOWN, this);
		MouseManager::getManager()->removeEventListener(Mouse::MOUSE_DOWN, this);
	}

	Renderable *TextInput::clone() const
	{
		return new TextInput(*this);
	}

	void TextInput::onEvent(MouseEvent *e)
	{
		if (!e)
		{
			return;
		}
		TextField *target = dynamic_cast<TextField *>(e->getOriginalTarget());
		setFocus(target == mText);
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
			// Backspace
			if (key == 8)
			{
				string str = getText();
				if (str.size() == 0 || mInputPosition == 0)
				{
					return;
				}

				if (ctrlDown)
				{
					int count = Utils::countWordLetters(getText(), mInputPosition, false);
					if (count != 0)
					{
						str.erase(mInputPosition + count, -count);
						mText->setText(str);
						fireChangeEvent();
						updateInputCursor(count);
					}
				}
				else
				{
					str.erase(mInputPosition - 1, 1);
					mText->setText(str);
					fireChangeEvent();
					updateInputCursor(-1);
				}
				return;
			}
			// Delete
			if (key == 46)
			{
				string str = getText();
				if (str.size() == 0 || mInputPosition == 0)
				{
					return;
				}

				if (ctrlDown)
				{
					int count = Utils::countWordLetters(getText(), mInputPosition, true);
					if (count != 0)
					{
						str.erase(mInputPosition, count);
						mText->setText(str);
						fireChangeEvent();
						//updateInputCursor(count);
					}
				}
				else
				{
					str.erase(mInputPosition, 1);
					mText->setText(str);
					fireChangeEvent();
					//updateInputCursor(0);
				}
				return;
			}
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
			// Up key
			else if (key == 38)
			{
				int position = mText->getLineAbove(mInputPosition);
				if (position >= 0)
				{
					setInputPosition(position);
				}
			}
			// Down key
			else if (key == 40)
			{
				int position = mText->getLineBelow(mInputPosition);
				if (position >= 0)
				{
					setInputPosition(position);
				}
			}
		}
		else
		{
			appendText(e->getKey());
			fireChangeEvent();
		}
	}

	void TextInput::setText(const char *text)
	{
		setText(string(text));
		fireChangeEvent();
	}
	void TextInput::setText(const string &text)
	{
		if (mMaxCharacters > 0 && text.size() > mMaxCharacters)
		{
			string temp(text);
			temp.resize(mMaxCharacters);
			mText->setText(temp);
			mInputPosition = temp.length();
		}
		else
		{
			mText->setText(text);
			mInputPosition = text.length();
		}
	}
	string TextInput::getTextStr() const
	{
		return mText->getText();
	}
	const char *TextInput::getText() const
	{
		return mText->getText();
	}

	void TextInput::appendText(char text)
	{
		if (mMaxCharacters > 0 && mText->length() >= mMaxCharacters || !validateChar(text))
		{
			return;
		}
		if (text == 8 || text == 127)
		{
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
		fireChangeEvent();
		updateInputCursor(1);
	}
	void TextInput::appendText(const char *text)
	{
		if (!validateText(text))
		{
			return;
		}
		string str = getText();
		str.insert(mInputPosition, text);
		if (mMaxCharacters > 0 && str.size() > mMaxCharacters)
		{
			str.resize(mMaxCharacters);
		}
		mText->setText(str);
		fireChangeEvent();
		updateInputCursor(strlen(text));
	}
	void TextInput::appendText(const string &text)
	{
		if (!validateText(text.c_str()))
		{
			return;
		}
		string str = getText();
		str.insert(mInputPosition, text);
		if (mMaxCharacters > 0 && str.size() > mMaxCharacters)
		{
			str.resize(mMaxCharacters);
		}
		mText->setText(str);
		fireChangeEvent();
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
		int maxPos = static_cast<int>(getTextStr().length());
		pos = pos > maxPos ? maxPos : pos;
		mInputPosition = pos;
		mText->setCursorInputPosition(pos);
	}

	void TextInput::setRestriction(TextInput::Restriction restriction)
	{
		mRestriction = restriction;
	}
	TextInput::Restriction TextInput::getRestriction() const
	{
		return mRestriction;
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
				KeyboardManager::getManager()->removeEventListener(Keyboard::KEY_PRESS, this);
			}
			else
			{
				KeyboardManager::getManager()->addEventListener(Keyboard::KEY_PRESS, this);
			}
			mFocus = focus;
			mText->setBlinkedEnabled(mFocus);
		}
	}
	bool TextInput::hasFocus() const
	{
		return mFocus;
	}

	void TextInput::setMaxCharacters(int maxCharacters, bool deleteExtra)
	{
		mMaxCharacters = maxCharacters;
		if (deleteExtra && maxCharacters > mText->length())
		{
			string text(mText->getText());
			text.resize(maxCharacters);
			mText->setText(text);
		}
	}
	int TextInput::getMaxCharacters() const
	{
		return mMaxCharacters;
	}

	void TextInput::setWidth(float width)
	{
		mText->setWidth(width);
	}
	void TextInput::setHeight(float height)
	{
		mText->setHeight(height);
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

	void TextInput::preRender(float dt)
	{
		UIComponent::preRender(dt);
		Texture::bindTexture(0);
		glBegin(GL_QUADS);
			glColor4d(0.2, 0.2, 0.2, mFocus ? 0.4 : 0.2);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(getWidth(), 0.0f);
			glVertex2f(getWidth(), getHeight());
			glVertex2f(0.0f, getHeight());
		glEnd();
		glBegin(GL_LINE_LOOP);
			glColor4d(0.7, 0.7, 0.7, mFocus ? 0.7 : 0.2);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(getWidth(), 0.0f);
			glVertex2f(getWidth(), getHeight());
			glVertex2f(0.0f, getHeight());
		glEnd();
	}

	void TextInput::fireChangeEvent()
	{
		am::base::Handle<Event> e(new Event("changed"));
		fireEvent<Event>(e);
	}

	bool TextInput::validateChar(char input)
	{
		if (mRestriction == NONE)
		{
			return true;
		}
		if (mRestriction == INTEGER)
		{
			return (input >= '0' && input <= '9') || input == '-';
		}
		return (input >= '0' && input <= '9') || input == '.' || input == '-';
	}
	bool TextInput::validateText(const char *input)
	{
		if (!input)
		{
			return false;
		}
		if (mRestriction == NONE)
		{
			return true;
		}
		char c = input[0];
		int i = 0;
		while (c != '\0')
		{
			if (!validateChar(c))
			{
				return false;
			}
			c = input[++i];
		}
		return true;
	}

}
}
