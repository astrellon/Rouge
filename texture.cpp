#include "texture.h"

#include "IL/il.h"

namespace am {
namespace base {

TextureManager Texture::sTextureManager;

TextureManager::TextureManager() :
	mValid(true)
{

}
TextureManager::~TextureManager()
{
	mValid = false;
}

bool TextureManager::altTextureRef(GLuint textureId, int ref)
{
	if (!mValid)
	{
		return false;
	}
	if (ref == 0) 
	{
		return false;
	}
	
	TextureRefCountMap::iterator iter = mTextureRefCounts.find(textureId);
	if (iter == mTextureRefCounts.end())
	{
		if (ref > 0)
		{
			mTextureRefCounts[textureId] = ref;
		}
		return false;
	}

	iter->second += ref;
	if (iter->second <= 0)
	{
		glDeleteTextures(1, &iter->first);
		return true;
	}
	return false;
}

void TextureManager::assignLoadedTexture(const char *filename, Texture *texture)
{
	if (!mValid)
	{
		return;
	}
	mLoadedTextures[string(filename)] = texture;
}
void TextureManager::removeLoadedTexture(const char *filename)
{
	if (!mValid)
	{
		return;
	}
	StoredTextureMap::iterator iter = mLoadedTextures.find(string(filename));
	if (iter != mLoadedTextures.end())
	{
		mLoadedTextures.erase(iter);
	}
}

Texture *TextureManager::getLoadedTexture(const char *filename)
{
	if (!mValid)
	{
		return NULL;
	}
	string fileStr = filename;
	StoredTextureMap::iterator iter = mLoadedTextures.find(fileStr);
	if (iter == mLoadedTextures.end())
	{
		return NULL;
	}
	return iter->second;
}



Texture::Texture() :
	mTextureId(0),
	mLoaded(false),
	mWidth(-1),
	mHeight(-1),
	mBytesPerPixel(-1)
{
}

Texture::Texture(const char *filename) :
	mTextureId(0),
	mLoaded(false),
	mWidth(-1),
	mHeight(-1),
	mBytesPerPixel(-1)
{
	loadFromFile(filename);
}
Texture::~Texture()
{
	destroy();
}

int Texture::loadFromFile(const char *filename)
{

	if (filename == NULL || filename[0] == '\0')
	{
		return -1;
	}

	destroy();

	const Texture *storedTexture = sTextureManager.getLoadedTexture(filename);
	if (storedTexture != NULL)
	{
		assign(*storedTexture);
		return 0;
	}

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

	int format = GL_RGBA;
	if (mBytesPerPixel == 3)
	{
		format = GL_RGB;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, ilGetData());

	ilDeleteImage(imgLoad);

	mLoaded = true;

	Texture *stored = new Texture();
	stored->assign(*this);

	sTextureManager.assignLoadedTexture(filename, stored);

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

void Texture::assign(const Texture &rhs)
{
	mTextureId = rhs.mTextureId;
	mLoaded = rhs.mLoaded;
	mWidth = rhs.mWidth;
	mHeight = rhs.mHeight;
	mBytesPerPixel = rhs.mBytesPerPixel;
	mFilename = rhs.mFilename;
	sTextureManager.altTextureRef(mTextureId, 1);
}


void Texture::destroy() 
{
	if (!mLoaded) {
		return;
	}

	if (sTextureManager.altTextureRef(mTextureId, -1))
	{
		sTextureManager.removeLoadedTexture(mFilename.c_str());
	}

	mLoaded = false;
	mTextureId = 0;
}

}
}
