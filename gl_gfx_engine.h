#pragma once

#include <vector>

#include "igfx_engine.h"

#include "resource_manager.h"

namespace am {
namespace gfx {

	class IGlRenderable;

	class GlAsset;
	class GlTexture;
	class GlFont;
	class GlSprite;
	class GlLayer;

	class GlGfxEngine : public IGfxEngine {
	public:
		// GlGfxEngine methods
		GlGfxEngine();
		~GlGfxEngine();

		GlAsset *getGlAsset(const char *assetName);
		GlTexture *getGlTexture(const char *textureName);
		GlFont *getGlFont(const char *fontName);

		GlSprite *getGlCursor();
		void setGlCursor(GlSprite *cursor);

		//void addObject(IGlRenderable *renderable);
		//void removeObject(IGlRenderable *renderable);

		// IGfxEngine methods
		virtual void init();
		virtual void deinit();
		virtual void setPerspective();
		virtual void setOrthographic();
	
		virtual void display(float dt);
		virtual void reshape(int width, int height);

		virtual IAsset *getAsset(const char *assetName);
		virtual ITexture *getTexture(const char *textureName);
		virtual IFont *getFont(const char *fontName);

		virtual ISprite *getCursor();
		virtual void setCursor(ISprite *cursor);

		virtual ILayer *getRootLayer();

		virtual ISprite *createSprite(IAsset *asset);
		virtual ISprite *createSprite(const string &assetName);
		virtual ILayer *createLayer();
		virtual ITextField *createTextField();
		virtual ITextList *createTextList();
		
	protected:
		
		int mScreenWidth;
		int mScreenHeight;

		GlSprite *mCursor;

		//typedef am::util::ResourceManager<GlTexture *, string> TextureManager;
		typedef map<string, GlAsset *> AssetManager;
		typedef map<string, GlTexture *> TextureManager;
		typedef map<string, GlFont *> FontManager;

		AssetManager mAssetManager;
		TextureManager mTextureManager;
		FontManager mFontManager;

		//vector<IGlRenderable *> mRenderables;
		GlLayer *mRootLayer;
	};

}
}
