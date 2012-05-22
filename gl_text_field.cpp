#include "gl_text_field.h"

#include "gl.h"

#include "gl_gfx_engine.h"
#include "gl_texture.h"
#include "gl_font.h"

namespace am {
namespace gfx {

	GlTextField::GlTextField(GlGfxEngine *engine) :
		mGfxEngine(engine),
		mText(""),
		mFont(NULL),
		mWidth(0.0f),
		mHeight(0.0f),
		mRenderedWidth(0),
		mRenderedHeight(0)
	{
		mTransform.setUpDirection(am::math::Transform::REF_FORWARD);
	}

	GlTextField::~GlTextField()
	{

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

	float GlTextField::getRenderedHeight() const
	{
		return mRenderedHeight;
	}

	void GlTextField::setText(string &str)
	{
		mText = str;
	}
	void GlTextField::appendText(string &str)
	{
		mText.append(str);
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

		glPushMatrix();
		glMultMatrixf(mTransform.data());

		glBindTexture(GL_TEXTURE_2D, mFont->getGlTexture()->getTextureId());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_QUADS);

			TextureWindow charRender;
			float xPos = 0.0f;
			float yPos = 0.0f;

			int len = static_cast<int>(mText.size());
			bool inWord = false;
			for (int i = 0; i < len; i++)
			{
				char ch = mText[i];
				if (ch <= ' ' && inWord)
				{
					inWord = false;
				}

				if (ch == ' ')
				{
					xPos += mFont->getSpaceWidth();
					continue;
				}
				else if(ch == '\t')
				{
					xPos = mFont->getVariableTabPosition(xPos);
					continue;
				}
				else if(ch == '\n')
				{
					xPos = 0.0f;
					yPos += mFont->getCharHeight() + mFont->getLeading();
					continue;
				}
				else if (mWidth > 0.0f && ch > ' ' && !inWord)
				{
					inWord = true;
					float wordWidth;
					float wordHeight;
					mFont->measureWord(mText.c_str() + i, wordWidth, wordHeight);
					if (xPos + wordWidth > mWidth)
					{
						xPos = 0.0f;
						yPos += mFont->getCharHeight() + mFont->getLeading();
					}
				}
				
				mFont->getTextureWindow(ch, charRender);

				glTexCoord2f(charRender.getLeftX(), charRender.getTopY());
				glVertex2f(xPos, yPos);
				
				glTexCoord2f(charRender.getRightX(), charRender.getTopY());
				glVertex2f(xPos+charRender.getWidth(), yPos);

				glTexCoord2f(charRender.getRightX(), charRender.getBottomY());
				glVertex2f(xPos + charRender.getWidth(), yPos + charRender.getHeight());

				glTexCoord2f(charRender.getLeftX(), charRender.getBottomY());
				glVertex2f(xPos, yPos + charRender.getHeight());

				xPos += charRender.getWidth() + mFont->getKerning();
			}
		glEnd();

		mRenderedHeight = yPos + mFont->getCharHeight();

		glDisable(GL_BLEND);
		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
	}

	am::math::Transform &GlTextField::getTransform()
	{
		return mTransform;
	}

	void GlTextField::setWidth(float width)
	{
		mWidth = width;
	}
	float GlTextField::getWidth() const
	{
		return mWidth;
	}

	void GlTextField::setHeight(float height)
	{
		mHeight = height;
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

}
}
