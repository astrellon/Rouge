#pragma once

#include <vector>
#include <map>
#include <string>

#include <base/handle.h>
#include <base/return_codes.h>

#include <util/colour.h>
#include <util/idefinition_manager.h>

#include <math/matrix.h>

namespace am {
namespace gfx {

	class Renderable;
	class Asset;
	class Texture;
	class Font;
	class Sprite;
	class Layer;
    class Camera;

	typedef std::map< std::string, base::Handle<Asset> > AssetMap;
	typedef std::map< std::string, base::Handle<Texture> > TextureMap;
	typedef std::map< std::string, base::Handle<Font> > FontMap;

	class GfxEngine : public util::IDefinitionManager 
	{
	public:
		virtual ~GfxEngine();

		// TODO!
		//int reloadAsset(const char *assetName);
		AssetMap &getAssetMap();

		void addAsset(Asset *asset);
		// Looks for the asset with the given name, will not automatically load the asset from file if it is not found.
		Asset *findAsset(const char *name);
		Asset *getAsset(const char *name, bool reload=false);

		base::ReturnCode getTexture(const char *textureName, Texture *&texture);
		//int reloadTexture(const char *textureName);
		TextureMap &getTextureMap();

		Font *getFont(const char *fontName);
		void addFont(Font *font);
		int reloadFont(const char *fontName);
		FontMap &getFontMap();

		Renderable *getCursor();
		void setCursor(Renderable *cursor);
		
		Renderable *getDefaultCursor();

		void init();
		void deinit();
		void setPerspective();
		void setOrthographic();
	
		void display(float dt);
		void reshape(int width, int height);

		int getScreenWidth() const;
		int getScreenHeight() const;

		Layer *getRootLayer();
		Layer *getGameLayer();
		Layer *getUILayer();
		Layer *getTooltipLayer();
		Layer *getDebugLayer();

		void setCursorHidden(bool hide);
		bool isCursorHidden() const;

		void setCameraLocation(float x, float y);
		float getCameraX() const;
		float getCameraY() const;
        void setCamera(Camera *camera);
        Camera *getCamera() const;

		void onKeyDown(const bool *keys, int key);
		void onKeyUp(const bool *keys, int key);

		void applyColourStack();
		void pushColourStack(const util::Colour &colour);
		void popColourStack();

		void setForceReloadMode(bool reload);
		bool isForeReloadMode() const;

		static GfxEngine *getEngine();
		static void deinitGfxEngine();

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:
		
		int mScreenWidth;
		int mScreenHeight;

		float mCameraX;
		float mCameraY;
        base::Handle<Camera> mCamera;

		base::Handle<Renderable> mCursor;
		base::Handle<Renderable> mDefaultCursor;
		bool mHideCursor;
		bool mForceReloadMode;

		AssetMap mAssetManager;
		TextureMap mTextureManager;
		FontMap mFontManager;

		base::Handle<Layer> mRootLayer;
		base::Handle<Layer> mGameLayer;
		base::Handle<Layer> mUILayer;
		base::Handle<Layer> mTooltipLayer;
		base::Handle<Layer> mDebugLayer;


		std::vector<util::Colour> mColourStack;

		static GfxEngine *sMainGfxEngine;

		GfxEngine();

		virtual const char *getBaseDefinitionPath(int id) const;
	};

}
}
