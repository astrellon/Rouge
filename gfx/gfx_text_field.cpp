#include "gfx_text_field.h"

#include "../gl.h"

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
		mDirty(true),
		mAlignment(ALIGN_LEFT)
	{
		//mTransform.setUpDirection(am::math::Transform::REF_FORWARD);
		mFont = GfxEngine::getGfxEngine()->getFont("basic");
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
		mFont = GfxEngine::getGfxEngine()->getFont(fontName);
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
	}
	void TextField::setText(const string &str)
	{
		mText = str;
		mDirty = true;
	}
	void TextField::appendText(const char *str)
	{
		mText.append(str);
		mDirty = true;
	}
	void TextField::appendText(const string &str)
	{
		mText.append(str);
		mDirty = true;
	}
	const string &TextField::getText() const
	{
		return mText;
	}
	string TextField::getText()
	{
		return mText;
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

		preRender(dt);
		renderText(mText);
		postRender(dt);
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

	void TextField::newLine()
	{
		mCurrXpos = 0.0f;
		mCurrYpos += mFont->getCharHeight() + mFont->getLeading();
	}
	void TextField::renderText(const string &text)
	{
		int len = static_cast<int>(text.size());
		for (int i = 0; i < len; i++)
		{
			char ch = text[i];
			if (ch <= ' ' && mInWord)
			{
				mInWord = false;
			}

			if (ch == ' ')
			{
				mCurrXpos += mFont->getSpaceWidth();
				continue;
			}
			else if(ch == '\t')
			{
				mCurrXpos = mFont->getVariableTabPosition(mCurrXpos);
				continue;
			}
			else if(ch == '\n')
			{
				newLine();
				checkAlignment(text.c_str() + i + 1);
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

}
}
