#include "TextField.h"

TextField::TextField()
{
	init();
}

TextField::TextField(ILubyte fontId)
{
	init();
	setFontId(fontId);
}

TextField::~TextField(void)
{
	if(m_temp != NULL)
	{
		delete []m_temp;
	}
}

void TextField::init()
{
	m_colour = 0xFF0000;
	m_temp = NULL;
	m_char_width = 0;
	m_char_height = 0;

	m_texture_width = 0;
	m_texture_height = 0;

	m_dirty = 0;
}

void TextField::render(int x, int y)
{
	glPushMatrix();
	glTranslatef((float)x, (float)y, 0);

	glBindTexture(GL_TEXTURE_2D, getTextureId());

	glBegin(GL_QUADS);

		glColor3f(0.3f, 0.3f, 0.3f);
		glTexCoord2f(0, 0);	glVertex2i(1, 1);
		glTexCoord2f(1, 0);	glVertex2i(m_texture_width + 1, 1);
		glTexCoord2f(1, 1);	glVertex2i(m_texture_width + 1, m_texture_height + 1);
		glTexCoord2f(0, 1);	glVertex2i(1, m_texture_height + 1);

		glColor3ub((m_colour >> 16) & 0xFF, (m_colour >> 8) & 0xFF, m_colour & 0xFF);
		glTexCoord2f(0, 0);	glVertex2i(0, 0);
		glTexCoord2f(1, 0);	glVertex2i(m_texture_width, 0);
		glTexCoord2f(1, 1);	glVertex2i(m_texture_width, m_texture_height);
		glTexCoord2f(0, 1);	glVertex2i(0, m_texture_height);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	glPopMatrix();
}

inline void TextField::setFontId(ILubyte fontId)
{
	if(m_temp != NULL)
	{
		delete []m_temp;
	}
	m_font_id = fontId;
	ilBindImage(m_font_id);

	m_char_width = (float)ilGetInteger(IL_IMAGE_WIDTH) / 16.0f;
	m_char_height = (float)ilGetInteger(IL_IMAGE_HEIGHT) / 16.0f;

	m_temp = new ILubyte[(int)ceil(m_char_width * m_char_height) * 4];
}

void TextField::blit(char letter, int x, int y)
{
	int srcX = letter & 0x0F;
	int srcY = (letter & 0xF0) >> 4;
	
	ilBindImage(m_font_id);

	ilCopyPixels((ILuint)(srcX * m_char_width), (ILuint)(srcY * m_char_height), 0, (ILuint)m_char_width, (ILuint)m_char_height, 1, IL_RGBA, IL_UNSIGNED_BYTE, m_temp);

	ilBindImage(m_image_id);
	ilSetPixels((ILint)(x * m_char_width), (ILint)(y * m_char_height), 0, (ILuint)m_char_width, (ILuint)m_char_height, 1, IL_RGBA, IL_UNSIGNED_BYTE, m_temp);
}

void TextField::updateTexture()
{
	glDeleteTextures(1, &m_texture_id);
	
	glGenTextures(1, &m_texture_id);
	glBindTexture(GL_TEXTURE_2D, m_texture_id);

	ilBindImage(m_font_id);

	int bpp = ilGetInteger(IL_IMAGE_BPP);

	m_image_id = ilGenImage();
	ilBindImage(m_image_id);

	m_texture_width = (int)ceil(m_char_width) * m_text.length();
	m_texture_height = (int)ceil(m_char_height);

	ilTexImage(m_texture_width, m_texture_height, 1, bpp, IL_RGBA, IL_UNSIGNED_BYTE, NULL);

	for(unsigned int i = 0; i < m_text.length(); i++)
	{
		blit(m_text[i], i, 0);
	}

	ilBindImage(m_image_id);

	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_texture_width, m_texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

	ilDeleteImage(m_image_id);
	
	m_dirty = false;
}
