#include "gl_texture.h"

#include "igfx_engine.h"
#include "gl_gfx_engine.h"

#include "IL/il.h"

namespace am {
namespace gfx {

	GlTexture::GlTexture(const char *filename) :
		mTextureId(0),
		mLoaded(false),
		mWidth(-1),
		mHeight(-1),
		mBytesPerPixel(-1),
		mGlFormat(GL_RGB)
	{
		loadFromFile(filename);
	}
	GlTexture::~GlTexture()
	{
		//destroy();
	}

	int GlTexture::loadFromFile(const char *filename)
	{

		if (filename == NULL || filename[0] == '\0')
		{
			return -1;
		}

		destroy();

		mFilename = filename;

		ILuint imgLoad = ilGenImage();
		ilBindImage(imgLoad);

		if (!ilLoadImage(filename))
		{
			return -2;
		}

		glGenTextures(1, &mTextureId);
		glBindTexture(GL_TEXTURE_2D, mTextureId);

		mWidth = static_cast<int>(ilGetInteger(IL_IMAGE_WIDTH));
		mHeight = static_cast<int>(ilGetInteger(IL_IMAGE_HEIGHT));
		mBytesPerPixel = static_cast<int>(ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL));

		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		mGlFormat = GL_RGBA;
		if (mBytesPerPixel == 3)
		{
			mGlFormat = GL_RGB;
		}


		glTexImage2D(GL_TEXTURE_2D, 0, mGlFormat, mWidth, mHeight, 0, mGlFormat, GL_UNSIGNED_BYTE, ilGetData());

		ilDeleteImage(imgLoad);

		mLoaded = true;

		/*Texture *stored = new Texture();
		stored->assign(*this);

		sTextureManager.assignLoadedTexture(filename, stored);
		*/
		return 0;
	}
	GLuint GlTexture::getTextureId() const
	{
		return mTextureId;
	}

	int GlTexture::getWidth() const 
	{
		return mWidth;
	}
	int GlTexture::getHeight() const 
	{
		return mHeight;
	}
	int GlTexture::getBytesPerPixel() const 
	{
		return mBytesPerPixel;
	}
	int GlTexture::getGlFormat() const
	{
		return mGlFormat;
	}

	GlTexture &GlTexture::operator=(const GlTexture &rhs)
	{
		assign(rhs);
		return *this;
	}
	GlTexture &GlTexture::operator=(const GlTexture *rhs)
	{
		assign(*rhs);
		return *this;
	}

	bool GlTexture::isLoaded() const
	{
		return mLoaded;
	}

	void GlTexture::assign(const GlTexture &rhs)
	{
		mTextureId = rhs.mTextureId;
		mLoaded = rhs.mLoaded;
		mWidth = rhs.mWidth;
		mHeight = rhs.mHeight;
		mBytesPerPixel = rhs.mBytesPerPixel;
		mFilename = rhs.mFilename;
		mGlFormat = rhs.mGlFormat;
		//sTextureManager.altTextureRef(mTextureId, 1);
	}

	IGfxEngine *GlTexture::getGfxEngine()
	{
		return mGfxEngine;
	}
	GlGfxEngine *GlTexture::getGlGfxEngine()
	{
		return mGfxEngine;
	}

	void GlTexture::destroy() 
	{
		if (!mLoaded) {
			return;
		}
		/*
		if (sTextureManager.altTextureRef(mTextureId, -1))
		{
			sTextureManager.removeLoadedTexture(mFilename.c_str());
		}
		*/
		mLoaded = false;
		mTextureId = 0;
	}

}
}
