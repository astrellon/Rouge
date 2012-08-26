#include "gfx_text_field.h"

#include <util/text_tokeniser.h>
#include <util/utils.h>
using namespace am::util;

#include <log/logger.h>

#include <gl.h>

#include "gfx_engine.h"
#include "gfx_texture.h"
#include "gfx_font.h"
#include "gfx_asset.h"
#include "gfx_layer.h"

namespace am {
namespace gfx {

	TextField::TextField() :
		Renderable(),
		mText(""),
		mMeasuredWidth(0),
		mMeasuredHeight(0),
		mRenderedHeight(0),
		mCursorXpos(-1.0f),
		mCursorYpos(-1.0f),
		mCursorInputPosition(-1),
		mDirty(true),
		mNewLineDirty(true),
		mAlignment(ALIGN_LEFT)
	{
		mFont = GfxEngine::getEngine()->getFont("basic");
	}

	TextField::~TextField()
	{

	}

	float TextField::getRenderedHeight() const
	{
		return mRenderedHeight;
	}

	Font *TextField::getBaseFont()
	{
		return mFont;
	}
	void TextField::setBaseFont(Font *font)
	{
		mFont = font;
	}
	void TextField::setBaseFont(const char *fontName)
	{
		mFont = GfxEngine::getEngine()->getFont(fontName);
	}

	float TextField::getMeasuredWidth()
	{
		if (mDirty)
		{
			calcSize();
		}
		return mMeasuredWidth;
	}
	float TextField::getMeasuredHeight()
	{
		if (mDirty)
		{
			calcSize();
		}
		return mMeasuredHeight;
	}

	void TextField::setText(const char *str)
	{
		mText = str;
		mDirty = true;
		mNewLineDirty = true;
	}
	void TextField::setText(const string &str)
	{
		mText = str;
		mDirty = true;
		mNewLineDirty = true;
	}
	void TextField::appendText(const char *str)
	{
		mText.append(str);
		mDirty = true;
		mNewLineDirty = true;
	}
	void TextField::appendText(const string &str)
	{
		mText.append(str);
		mDirty = true;
		mNewLineDirty = true;
	}
	const char *TextField::getText() const
	{
		return mText.c_str();
	}
	int TextField::length() const
	{
		return static_cast<int>(mText.size());
	}

	void TextField::setAlignment(TextField::TextAlignment align)
	{
		mAlignment = align;
	}
	TextField::TextAlignment TextField::getAlignment() const
	{
		return mAlignment;
	}

	void TextField::render(float dt)
	{
		if (!mVisible || mFont == NULL || !mFont->isLoaded())
		{
			return;
		}

		if (mNewLineDirty)
		{
			mNewLinePositions.clear();
			mNewLinePositions.push_back(0);
		}
		mCursorXpos = -1.0f;
		preRender(dt);
		renderText(mText);
		postRender(dt);

		if (mCursorXpos >= 0.0f)
		{
			drawCursorInput();
		}
	}

	void TextField::calcSize()
	{
		mFont->measureText(mText.c_str(), mWidth, mMeasuredWidth, mMeasuredHeight);
		mDirty = false;
	}

	void TextField::preRender(float dt)
	{
		Renderable::preRender(dt);

		glBindTexture(GL_TEXTURE_2D, mFont->getAsset()->getTexture()->getTextureId());

		glBegin(GL_QUADS);

		mTextPosition = 0;

		mCurrXpos = 0.0f;
		mCurrYpos = 0.0f;
		checkAlignment(mText.c_str());
		mInWord = false;
	}

	void TextField::postRender(float dt)
	{
		glEnd();

		mRenderedHeight = mCurrYpos + mFont->getCharHeight();
		glBindTexture(GL_TEXTURE_2D, 0);
		
		Renderable::postRender(dt);
	}

	int TextField::getTextPosition(int localX, int localY) const
	{
		int line = localY / static_cast<int>(mFont->getCharHeight() + mFont->getLeading());
		if (line < 0)
		{
			return -1;
		}
		if (line >= mNewLinePositions.size())
		{
			return -2;
		}
		int index = mNewLinePositions[line];
		char ch = mText[index];
		float left = -mFont->getKerning();
		while (ch != '\0' && ch != '\n' && localX > left)
		{
			left += mFont->getCharWidth(ch) + mFont->getKerning();
			ch = mText[++index];
		}
		return index;
	}

	void TextField::newLine()
	{
		mCurrXpos = 0.0f;
		mCurrYpos += mFont->getCharHeight() + mFont->getLeading();

		if (mNewLineDirty)
		{
			mNewLinePositions.push_back(mTextPosition);
		}
	}
	void TextField::renderText(const string &text)
	{
		int len = static_cast<int>(text.size());
		if (mTextPosition == mCursorInputPosition)
		{
			mCursorXpos = mCurrXpos;
			mCursorYpos = mCurrYpos;
		}
		for (int i = 0; i < len; i++, mTextPosition++)
		{
			char ch = text[i];
			if (ch <= ' ' && mInWord)
			{
				mInWord = false;
			}

			if (ch == ' ')
			{
				mCurrXpos += mFont->getSpaceWidth();
				checkCursorInput();
				continue;
			}
			else if(ch == '\t')
			{
				mCurrXpos = mFont->getVariableTabPosition(mCurrXpos);
				checkCursorInput();
				continue;
			}
			else if(ch == '\n')
			{
				newLine();
				checkAlignment(text.c_str() + i + 1);
				checkCursorInput();
				continue;
			}
			else if (mWidth > 0.0f && ch > ' ' && !mInWord)
			{
				mInWord = true;
				float wordWidth = 0.0f;
				float wordHeight = 0.0f;
				mFont->measureWord(text.c_str() + i, wordWidth, wordHeight);
				if (mCurrXpos + wordWidth > mWidth)
				{
					newLine();
					checkAlignment(text.c_str() + i);
				}
				checkCursorInput();
			}
				
			mFont->getTextureWindow(ch, mCharRender);

			glTexCoord2f(mCharRender.getLeftX(), mCharRender.getTopY());
			glVertex2f(mCurrXpos, mCurrYpos);
				
			glTexCoord2f(mCharRender.getRightX(), mCharRender.getTopY());
			glVertex2f(mCurrXpos + mCharRender.getWidth(), mCurrYpos);

			glTexCoord2f(mCharRender.getRightX(), mCharRender.getBottomY());
			glVertex2f(mCurrXpos + mCharRender.getWidth(), mCurrYpos + mCharRender.getHeight());

			glTexCoord2f(mCharRender.getLeftX(), mCharRender.getBottomY());
			glVertex2f(mCurrXpos, mCurrYpos + mCharRender.getHeight());

			mCurrXpos += mCharRender.getWidth();
			if (text[i + 1] > ' ')
			{
				mCurrXpos += mFont->getKerning();
			}

			checkCursorInput();
		}

	}

	void TextField::setWidth(float width)
	{
		if (width != mWidth)
		{
			mDirty = true;
			Renderable::setWidth(width);
		}
	}
	void TextField::setHeight(float height)
	{
		if (height != mHeight)
		{
			mDirty = true;
			Renderable::setHeight(height);
		}
	}

	void TextField::checkAlignment(const char *line)
	{
		if (mAlignment == ALIGN_RIGHT)
		{
			float width = 0.0f;
			float height = 0.0f;
			mFont->measureLine(line, mWidth, width, height);
			mCurrXpos = mWidth - width;
		}
		else if (mAlignment == ALIGN_CENTER)
		{
			float width = 0.0f;
			float height = 0.0f;
			mFont->measureLine(line, mWidth, width, height);
			mCurrXpos = (mWidth - width) / 2;
		}
	}

	void TextField::setCursorInputPosition( int position )
	{
		mCursorInputPosition = position;
	}
	int TextField::getCursorInputPosition() const
	{
		return mCursorInputPosition;
	}

	void TextField::checkCursorInput()
	{
		if (mTextPosition + 1 == mCursorInputPosition)
		{
			mCursorXpos = mCurrXpos;
			mCursorYpos = mCurrYpos;
		}
	}

	void TextField::drawCursorInput()
	{
		glBegin(GL_QUADS);
			glVertex2f(mCursorXpos + 1, mCursorYpos);
			glVertex2f(mCursorXpos + 2, mCursorYpos);
			glVertex2f(mCursorXpos + 2, mCursorYpos + mFont->getCharHeight());
			glVertex2f(mCursorXpos + 1, mCursorYpos + mFont->getCharHeight());
		glEnd();
	}

	int TextField::getStartOfLine( int textPosition ) const
	{
		if (textPosition < 0)
		{
			return 0;
		}
		int len = static_cast<int>(mText.size());
		if (textPosition > len)
		{
			return mNewLinePositions.back();
		}
		
		if (mNewLinePositions.size() <= 1)
		{
			return 0;
		}
		int prev = 0;
		for (size_t i = 1; i < mNewLinePositions.size(); i++)
		{
			int curr = mNewLinePositions[i];
			if (textPosition > prev &&
				textPosition < curr)
			{
				return prev;
			}
			prev = curr;
		}
	}

	int TextField::getEndOfLine( int textPosition ) const
	{
		int len = static_cast<int>(mText.size());
		if (textPosition < 0 || textPosition >= len || mNewLinePositions.size() <= 1)
		{
			return len;
		}

		int prev = 0;
		for (size_t i = 1; i < mNewLinePositions.size(); i++)
		{
			int curr = mNewLinePositions[i];
			if (textPosition > prev &&
				textPosition < curr)
			{
				return curr - 1;
			}
			prev = curr;
		}
	}

}
}
