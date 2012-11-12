#pragma once

#include <gl.h>

#include <base/imanaged.h>
#include <string>

using namespace std;
using namespace am::base;

namespace am {
namespace gfx {

	class Texture : public IManaged {
	public:
		Texture(const char *filename, GLuint textureId);
		Texture(const char *filename);
		~Texture();

		void destroy();

		const char *getFilename() const;
		bool isLoaded() const;

		int reload();

		int getWidth() const;
		int getHeight() const;
		int getBytesPerPixel() const;
		int getGlFormat() const;

		GLuint getTextureId() const;
	
		Texture &operator=(const Texture &rhs);
		Texture &operator=(const Texture *rhs);

		void bindTexture() const;
		static void bindTexture(GLuint id);

	protected:

		int loadFromFile(const char *filename);

		GLuint mTextureId;
		bool mLoaded;

		int mWidth;
		int mHeight;
		int mBytesPerPixel;
		int mGlFormat;

		string mFilename;

		void assign(const Texture &rhs);

		static GLuint sLastBind;

	};

}
}
