#pragma once

#include "../gl.h"

#include "gfx_component.h"

#include <string>

using namespace std;

namespace am {
namespace gfx {

	class GfxEngine;

	class Texture : public GfxComponent {
	public:
		Texture(GfxEngine *engine, const char *filename, GLuint textureId);
		Texture(GfxEngine *engine, const char *filename);
		~Texture();

		void destroy();

		const char *getFilename() const;
		bool isLoaded() const;

		int getWidth() const;
		int getHeight() const;
		int getBytesPerPixel() const;
		int getGlFormat() const;

		GLuint getTextureId() const;
	
		Texture &operator=(const Texture &rhs);
		Texture &operator=(const Texture *rhs);

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

	};

}
}
