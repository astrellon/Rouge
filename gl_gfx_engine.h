#pragma once

#include "igfx_engine.h"

#include "resource_manager.h"

namespace am {
namespace gfx {

	class GlTexture;
	class GlFont;

	class GlGfxEngine : public IGfxEngine {
	public:
		GlGfxEngine();
		~GlGfxEngine();

		virtual void init();
		virtual void deinit();
		virtual void setPerspective();
		virtual void setOrthographic();
	
		virtual void display();
		virtual void reshape(int width, int height);

		virtual void loadFontDefinitions(const char *filename);

		virtual ITexture *loadTexture(const char *filename);
		GlTexture *loadGlTexture(const char *filename);
		virtual IFont *getFont(const char *fontName);
		GlFont *getGlFont(const char *fontName);

	protected:
		
		int mScreenWidth;
		int mScreenHeight;

		typedef am::util::ResourceManager<GlTexture, string> TextureManager;
		typedef map<string, GlFont *> FontManager;

		TextureManager mTextureManager;
		FontManager mFontManager;

	};

}
}
