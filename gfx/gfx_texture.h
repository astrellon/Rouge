#pragma once

#include <gl.h>

#include <base/return_codes.h>
#include <base/imanaged.h>
using namespace am::base;

#include <string>
using namespace std;

namespace am {
namespace gfx {

	class Texture : public IManaged {
	public:
		Texture();
		//Texture(const char *filename, GLuint textureId);
		Texture(const char *filename);
		~Texture();

		const char *getFilename() const;
		ReturnCode setFilename(const char *filename);
		bool isLoaded() const;

		int reload();

		int getWidth() const;
		int getHeight() const;
		int getBytesPerPixel() const;
		int getGlFormat() const;

		GLuint getTextureId() const;

		void bindTexture() const;
		static void bindTexture(GLuint id);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		ReturnCode loadFromFile(const char *filename);

		GLuint mTextureId;
		bool mLoaded;

		int mWidth;
		int mHeight;
		int mBytesPerPixel;
		int mGlFormat;

		string mFilename;

		void destroy(GLuint textureId);

		static GLuint sLastBind;

	};

}
}
