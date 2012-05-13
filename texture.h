#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <string>
#include <map>

using namespace std;

namespace am {
namespace base {

class Texture;

class TextureManager {
public:
	TextureManager();
	~TextureManager();

	Texture *getLoadedTexture(const char *filename);
	void assignLoadedTexture(const char *filename, Texture *texture);
	void removeLoadedTexture(const char *filename);
	bool altTextureRef(GLuint textureId, int ref);

private:
	typedef map<string, Texture *> StoredTextureMap;
	typedef map<GLuint, int> TextureRefCountMap;

	StoredTextureMap mLoadedTextures;
	TextureRefCountMap mTextureRefCounts;
	bool mValid;
};

class Texture {
public:
	Texture();
	Texture(const char *filename);
	~Texture();

	int loadFromFile(const char *filename);
	GLuint getTextureId() const;

	bool isLoaded() const;

	int getWidth() const;
	int getHeight() const;
	int getBytesPerPixel() const;
	int getGlFormat() const;

	Texture &operator=(const Texture &rhs);
	Texture &operator=(const Texture *rhs);
	
protected:
	GLuint mTextureId;
	bool mLoaded;

	int mWidth;
	int mHeight;
	int mBytesPerPixel;
	int mGlFormat;

	string mFilename;

	void destroy();

	void assign(const Texture &rhs);

private:
	static TextureManager sTextureManager;

};

}
}
