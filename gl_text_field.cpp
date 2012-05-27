#include "gl_text_field.h"

#include "gl.h"

#include "gl_gfx_engine.h"
#include "gl_texture.h"
#include "gl_font.h"
#include "gl_asset.h"

namespace am {
namespace gfx {

	GlTextField::GlTextField(GlGfxEngine *engine) :
		mGfxEngine(engine),
		mText(""),
		mWidth(0.0f),
		mHeight(0.0f),
		mMeasuredWidth(0),
		mMeasuredHeight(0),
		mRenderedHeight(0),
		mDirty(true)
	{
		mTransform.setUpDirection(am::math::Transform::REF_FORWARD);
		mFont = engine->getGlFont("basic");
	}

	GlTextField::~GlTextField()
	{

	}

	float GlTextField::getRenderedHeight() const
	{
		return mRenderedHeight;
	}

	IFont *GlTextField::getBaseFont()
	{
		return mFont;
	}
	void GlTextField::setBaseFont(IFont *font)
	{
		GlFont *fontCheck = dynamic_cast<GlFont *>(font);
		if (fontCheck != NULL)
		{
			mFont = fontCheck;
		}
	}
	GlFont *GlTextField::getBaseGlFont()
	{
		return mFont;
	}
	void GlTextField::setBaseGlFont(GlFont *font)
	{
		mFont = font;
	}

	float GlTextField::getMeasuredWidth()
	{
		if (mDirty)
		{
			calcSize();
		}
		return mMeasuredWidth;
	}
	float GlTextField::getMeasuredHeight()
	{
		if (mDirty)
		{
			calcSize();
		}
		return mMeasuredHeight;
	}

	void GlTextField::setText(string &str)
	{
		mText = str;
		mDirty = true;
	}
	void GlTextField::appendText(string &str)
	{
		mText.append(str);
		mDirty = true;
	}
	string GlTextField::getText()
	{
		return mText;
	}

	void GlTextField::render(float dt)
	{
		if (mFont == NULL || !mFont->isLoaded())
		{
			return;
		}

		preRender();
		renderText(mText);
		postRender();
	}

	am::math::Transform &GlTextField::getTransform()
	{
		return mTransform;
	}

	void GlTextField::setWidth(float width)
	{
		mWidth = width;
		mDirty = true;
	}
	float GlTextField::getWidth() const
	{
		return mWidth;
	}

	void GlTextField::setHeight(float height)
	{
		mHeight = height;
		mDirty = true;
	}
	float GlTextField::getHeight() const
	{
		return mHeight;
	}

	IGfxEngine *GlTextField::getGfxEngine()
	{
		return mGfxEngine;
	}
	GlGfxEngine *GlTextField::getGlGfxEngine()
	{
		return mGfxEngine;
	}

	void GlTextField::calcSize()
	{
		mFont->measureText(mText.c_str(), mMeasuredWidth, mMeasuredHeight);
		mDirty = false;
	}

	void GlTextField::preRender()
	{
		glPushMatrix();
		glMultMatrixf(mTransform.data());

		glBindTexture(GL_TEXTURE_2D, mFont->getGlAsset()->getGlTexture()->getTextureId());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_QUADS);

		mCurrXpos = 0.0f;
		mCurrYpos = 0.0f;
		mInWord = false;
	}

	void GlTextField::postRender()
	{
		glEnd();

		mRenderedHeight = mCurrYpos + mFont->getCharHeight();

		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
	}

	void GlTextField::newLine()
	{
		mCurrXpos = 0.0f;
		mCurrYpos += mFont->getCharHeight() + mFont->getLeading();
	}
	void GlTextField::renderText(const string &text)
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
				continue;
			}
			else if (mWidth > 0.0f && ch > ' ' && !mInWord)
			{
				mInWord = true;
				float wordWidth;
				float wordHeight;
				mFont->measureWord(text.c_str() + i, wordWidth, wordHeight);
				if (mCurrXpos + wordWidth > mWidth)
				{
					newLine();
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

			mCurrXpos += mCharRender.getWidth() + mFont->getKerning();
		}
	}
}
}
