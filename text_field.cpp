#include "text_field.h"

#include "font.h"
#include "texture.h"

#include "gl.h"

namespace am {
namespace ui {

	TextField::TextField() :
		mText(""),
		mFont(NULL)
	{

	}

	TextField::TextField(Font *font) :
		mText(""),
		mFont(font)
	{

	}

	TextField::~TextField()
	{

	}

	Font *TextField::getFont()
	{
		return mFont;
	}
	void TextField::setFont(Font *font)
	{
		mFont = font;
	}

	void TextField::setText(string &str)
	{
		mText = str;
	}
	void TextField::appendText(string &str)
	{
		mText.append(str);
	}
	string TextField::getText()
	{
		return mText;
	}

	void TextField::render(int x, int y)
	{
		if (mFont == NULL || !mFont->isLoaded())
		{
			return;
		}
		glPushMatrix();
		glTranslatef((float)x, (float)y, 0);

		glBindTexture(GL_TEXTURE_2D, mFont->getTexture()->getTextureId());

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glBegin(GL_QUADS);

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

}
}
