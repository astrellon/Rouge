#pragma once

#include "gl.h"

#include "itexture.h"
#include "igl_gfx_component.h"

#include <string>

using namespace std;

namespace am {
namespace gfx {

	class GlGfxEngine;

	class GlTexture : public ITexture, public IGlGfxComponent {
	public:
		~GlTexture();

		virtual void destroy();

		virtual bool isLoaded() const;

		virtual int getWidth() const;
		virtual int getHeight() const;
		virtual int getBytesPerPixel() const;
		virtual int getGlFormat() const;

		virtual IGfxEngine *getGfxEngine();
		virtual GlGfxEngine *getGlGfxEngine();

		GLuint getTextureId() const;
	
		GlTexture &operator=(const GlTexture &rhs);
		GlTexture &operator=(const GlTexture *rhs);
	
		friend class GlGfxEngine;

	protected:

		GlTexture(GlGfxEngine *engine, const char *filename);

		int loadFromFile(const char *filename);

		GLuint mTextureId;
		bool mLoaded;
		GlGfxEngine *mGfxEngine;

		int mWidth;
		int mHeight;
		int mBytesPerPixel;
		int mGlFormat;

		string mFilename;

		void assign(const GlTexture &rhs);

	};

}
}
