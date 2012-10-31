#pragma once

#include <vector>
#include <map>
#include <string>
using namespace std;

#include <base/handle.h>
using namespace am::base;

#include <util/colour.h>
using namespace am::util;

#include <math/matrix.h>
using namespace am::math;

namespace am {
namespace gfx {

	class Renderable;
	class Asset;
	class Texture;
	class Font;
	class Sprite;
	class Layer;

	typedef map<string, Handle<Asset> > AssetMap;
	typedef map<string, Handle<Texture> > TextureMap;
	typedef map<string, Handle<Font> > FontMap;

	class GfxEngine {
	public:
		~GfxEngine();

		Asset *getAssetLua(const char *assetName);
		int reloadAsset(const char *assetName);
		AssetMap &getAssetMap();

		Texture *getTexture(const char *textureName);
		int reloadTexture(const char *textureName);
		TextureMap &getTextureMap();

		Font *getFontLua(const char *fontName);
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

		void onKeyDown(const bool *keys, int key);
		void onKeyUp(const bool *keys, int key);

		void applyColourStack();
		void pushColourStack(const Colour &colour);
		void popColourStack();

		static GfxEngine *getEngine();

	protected:
		
		int mScreenWidth;
		int mScreenHeight;

		float mCameraX;
		float mCameraY;

		Handle<Renderable> mCursor;
		Handle<Renderable> mDefaultCursor;
		bool mHideCursor;

		AssetMap mAssetManager;
		TextureMap mTextureManager;
		FontMap mFontManager;

		Handle<Layer> mRootLayer;
		Handle<Layer> mGameLayer;
		Handle<Layer> mUILayer;
		Handle<Layer> mTooltipLayer;
		Handle<Layer> mDebugLayer;

		vector<Colour> mColourStack;

		static GfxEngine *sMainGfxEngine;

		GfxEngine();
	};

}
}
