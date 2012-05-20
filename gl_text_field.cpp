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
		mFont(NULL)
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
		mTransform.translate(0.1f, 0, 0, true);
		//glTranslatef(x, y, 0);
		glMultMatrixf(mTransform.data());

		glBindTexture(GL_TEXTURE_2D, mFont->getGlTexture()->getTextureId());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_QUADS);

			CharRender charRender;
			float xPos = 0.0f;
			float yPos = 0.0f;

			int len = static_cast<int>(mText.size());
			for (int i = 0; i < len; i++)
			{
				char ch = mText[i];
				if (ch == ' ')
				{
					xPos += mFont->getSpaceWidth();
					continue;
				}
				else if(ch == '\t')
				{
					int xMult = static_cast<int>(xPos) / static_cast<int>(mFont->getTabWidth());
					float nextXpos = static_cast<float>(xMult + 1) * mFont->getTabWidth();
					if (nextXpos - xPos < mFont->getSpaceWidth())
					{
						nextXpos += mFont->getSpaceWidth();
					}
					xPos = nextXpos;
					continue;
				}
				else if(ch == '\n')
				{
					xPos = 0.0f;
					yPos += mFont->getCharHeight() + mFont->getLeading();
					continue;
				}
				mFont->getCharRender(ch, charRender);

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

		glDisable(GL_BLEND);

		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
	}

	am::math::Transform &GlTextField::getTransform()
	{
		return mTransform;
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
