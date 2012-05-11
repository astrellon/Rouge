#pragma once

#include <Windows.h>
#include <gl/GL.h>
#include <string>
#include <map>

using namespace std;

namespace am {
namespace base {

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
	
protected:
	GLuint mTextureId;
	bool mLoaded;

	int mWidth;
	int mHeight;
	int mBytesPerPixel;
	int mRefCount;

	void destroy();

private:


	//typedef pair<GLuint, int> StoredTextureId;
	//typedef map<string, StoredTextureId> StoredTextureMap;
	typedef map<string, GLuint> StoredTextureMap;
	typedef map<GLuint, int> TextureRefCountMap;

	static StoredTextureMap sLoadedTextures;
	static TextureRefCountMap sTextureRefCounts;

	static GLuint getLoadedTexture(const char *filename);
	static void altTextureRef(GLuint textureId, int ref);

};

}
}
