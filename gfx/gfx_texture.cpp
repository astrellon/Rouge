#include "gfx_texture.h"

#include "IL/il.h"

#include <util/utils.h>
using namespace am::util;

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace gfx {

	GLuint Texture::sLastBind = -1;

	const int Texture::LUA_ID = LUA_ID_TEXTURE;
	const char *Texture::LUA_TABLENAME = LUA_TABLE_TEXTURE;

	Texture::Texture() :
		base::IManaged(),
		mTextureId(0),
		mWidth(-1),
		mHeight(-1),
		mBytesPerPixel(-1),
		mGlFormat(GL_RGB),
		mLoaded(false)
	{
	}

	/*Texture::Texture(const char *filename, GLuint textureId) :
		base::IManaged(),
		mTextureId(textureId),
		mFilename(filename),
		mLoaded(false)
	{
		// TODO: Get GL width and height, etc
	}*/
	Texture::Texture(const char *filename) :
		base::IManaged(),
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
		destroy(mTextureId);
	}

	bool Texture::isLoaded() const
	{
		return mLoaded;
	}

	base::ReturnCode Texture::loadFromFile(const char *filename)
	{
		if (filename == nullptr || filename[0] == '\0')
		{
			return base::NULL_PARAMETER;
		}

		if (!Utils::fileExists(filename))
		{
			return base::FILE_NOT_FOUND;
		}

		GLuint prevId = mTextureId;

		ILuint imgLoad = ilGenImage();
		ilBindImage(imgLoad);

		if (!ilLoadImage(filename))
		{
			return base::TEXTURE_LOAD_FAILED;
		}

		mFilename = filename;

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
		destroy(prevId);

		return base::SUCCESS;
	}
	GLuint Texture::getTextureId() const
	{
		return mTextureId;
	}

	int Texture::reload()
	{
		return loadFromFile(mFilename.c_str());
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

	const char *Texture::getFilename() const
	{
		return mFilename.c_str();
	}
	base::ReturnCode Texture::setFilename(const char *filename)
	{
		return loadFromFile(filename);
	}

	void Texture::destroy(GLuint textureId) 
	{
		if (textureId == 0) {
			return;
		}

		GLuint textures[1] = {textureId};
		glDeleteTextures(1, textures);
	}

	void Texture::bindTexture() const
	{
		if (sLastBind != mTextureId)
		{
			sLastBind = mTextureId;
			glBindTexture(GL_TEXTURE_2D, mTextureId);
		}
	}
	void Texture::bindTexture(GLuint id)
	{
		if (sLastBind != id)
		{
			sLastBind = id;
			glBindTexture(GL_TEXTURE_2D, id);
		}
	}

}
}
