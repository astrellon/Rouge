#include "gfx_texture.h"

#include "IL/il.h"

namespace am {
namespace gfx {

	Texture::Texture(const char *filename, GLuint textureId) :
		IManaged(),
		mTextureId(textureId),
		mFilename(filename),
		mLoaded(false)
	{
		// TODO: Get GL width and height, etc
	}
	Texture::Texture(const char *filename) :
		IManaged(),
		mTextureId(0),
		mWidth(-1),
		mHeight(-1),
		mBytesPerPixel(-1),
		mGlFormat(GL_RGB),
		mLoaded(false)
	{
		loadFromFile(filename);
	}
	Texture::~Texture()
	{
		//destroy();
	}

	bool Texture::isLoaded() const
	{
		return mLoaded;
	}

	int Texture::loadFromFile(const char *filename)
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

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		mGlFormat = GL_RGBA;
		if (mBytesPerPixel == 3)
		{
			mGlFormat = GL_RGB;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, mGlFormat, mWidth, mHeight, 0, mGlFormat, GL_UNSIGNED_BYTE, ilGetData());

		ilDeleteImage(imgLoad);

		mLoaded = true;

		return 0;
	}
	GLuint Texture::getTextureId() const
	{
		return mTextureId;
	}

	int Texture::getWidth() const 
	{
		return mWidth;
	}
	int Texture::getHeight() const 
	{
		return mHeight;
	}
	int Texture::getBytesPerPixel() const 
	{
		return mBytesPerPixel;
	}
	int Texture::getGlFormat() const
	{
		return mGlFormat;
	}

	Texture &Texture::operator=(const Texture &rhs)
	{
		assign(rhs);
		return *this;
	}
	Texture &Texture::operator=(const Texture *rhs)
	{
		assign(*rhs);
		return *this;
	}

	const char *Texture::getFilename() const
	{
		return mFilename.c_str();
	}

	void Texture::assign(const Texture &rhs)
	{
		mTextureId = rhs.mTextureId;
		mWidth = rhs.mWidth;
		mHeight = rhs.mHeight;
		mBytesPerPixel = rhs.mBytesPerPixel;
		mFilename = rhs.mFilename;
		mGlFormat = rhs.mGlFormat;
		//sTextureManager.altTextureRef(mTextureId, 1);
	}

	void Texture::destroy() 
	{
		if (mTextureId > 0) {
			return;
		}
		/*
		if (sTextureManager.altTextureRef(mTextureId, -1))
		{
			sTextureManager.removeLoadedTexture(mFilename.c_str());
		}
		*/
		mFilename = "";
		mTextureId = 0;
		mLoaded = false;
	}

}
}
