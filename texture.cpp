#include "texture.h"

#include "IL/il.h"

namespace am {
namespace base {

StoredTextureMap Texture::sLoadedTextures;

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
Texture::~Texture() {
	destroy();
}

int Texture::loadFromFile(const char *filename) {

	if (filename == NULL || filename[0] == '\0') {
		return -1;
	}

	destroy();

	const Texture *storedTexture = getLoadedTexture(filename);

	ILuint imgLoad = ilGenImage();
	ilBindImage(imgLoad);

	if (!ilLoadImage(filename)) {
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
	if (mBytesPerPixel == 3) {
		format = GL_RGB;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, ilGetData());

	ilDeleteImage(imgLoad);

	mLoaded = true;

	return 0;
}
GLuint Texture::getTextureId() const {
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


void Texture::destroy() 
{
	if (!mLoaded) {
		return;
	}

	altTextureRef(mTextureId, -1);

	mLoaded = false;
	mTextureId = 0;
}

void Texture::altTextureRef(GLuint textureId, int ref)
{
	if (ref == 0) {
		return;
	}
	TextureRefCountMap::iterator iter = sTextureRefCounts.find(textureId);
	if (iter == sTextureRefCounts.end()) {
		return;
	}

	iter->second += ref;
	if (iter->second <= 0) {
		glDeleteTextures(1, &iter->first);
	}
}

}
}
