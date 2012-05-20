#pragma once

#include <vector>

#include "igfx_engine.h"

#include "resource_manager.h"

namespace am {
namespace gfx {

	class IGlRenderable;
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
	
		virtual void display(float dt);
		virtual void reshape(int width, int height);

		virtual int loadFontDefinitions(const char *filename);

		virtual ITexture *loadTexture(const char *filename);
		GlTexture *loadGlTexture(const char *filename);
		virtual IFont *getFont(const char *fontName);
		GlFont *getGlFont(const char *fontName);

		void addObject(IGlRenderable *renderable);
		void removeObject(IGlRenderable *renderable);

	protected:
		
		int mScreenWidth;
		int mScreenHeight;

		//typedef am::util::ResourceManager<GlTexture *, string> TextureManager;
		typedef map<string, GlTexture *> TextureManager;
		typedef map<string, GlFont *> FontManager;

		TextureManager mTextureManager;
		FontManager mFontManager;

		vector<IGlRenderable *> mRenderables;
	};

}
}
