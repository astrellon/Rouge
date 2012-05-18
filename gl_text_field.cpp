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

	void GlTextField::render(float x, float y)
	{
		if (mFont == NULL || !mFont->isLoaded())
		{
			return;
		}
		glPushMatrix();
		glTranslatef(x, y, 0);

		glBindTexture(GL_TEXTURE_2D, mFont->getGlTexture()->getTextureId());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_QUADS);

			/*glTexCoord2f(0, 0);
			glVertex2f(0, 0);

			glTexCoord2f(1, 0);
			glVertex2f(128, 0);

			glTexCoord2f(1, 1);
			glVertex2f(128, 128);

			glTexCoord2f(0, 1);
			glVertex2f(0, 128);
			*/
			CharRender charRender;
			float xPos = 0.0f;
			for (int i = 0; i < mText.size(); i++)
			{
				mFont->getCharRender(mText[i], charRender);

				glTexCoord2f(charRender.getLeftX(), charRender.getTopY());
				glVertex2f(xPos, 0);
				
				glTexCoord2f(charRender.getRightX(), charRender.getTopY());
				glVertex2f(xPos+charRender.getWidth(), 0);

				glTexCoord2f(charRender.getRightX(), charRender.getBottomY());
				glVertex2f(xPos+charRender.getWidth(), charRender.getHeight());

				glTexCoord2f(charRender.getLeftX(), charRender.getBottomY());
				glVertex2f(xPos, charRender.getHeight());

				xPos += charRender.getWidth() + 1;
			}
		glEnd();

		glDisable(GL_BLEND);

		glBindTexture(GL_TEXTURE_2D, 0);
		glPopMatrix();
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
