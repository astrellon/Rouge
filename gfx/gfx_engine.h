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

	class GfxEngine {
	public:
		~GfxEngine();

		Asset *getAsset(const char *assetName);
		int reloadAsset(const char *assetName);
		Texture *getTexture(const char *textureName);
		int reloadTexture(const char *textureName);
		Font *getFont(const char *fontName);

		Sprite *getCursor();
		void setCursor(Sprite *cursor);

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

		Handle<Sprite> mCursor;
		bool mHideCursor;

		typedef map<string, Handle<Asset> > AssetManager;
		typedef map<string, Handle<Texture> > TextureManager;
		typedef map<string, Handle<Font> > FontManager;

		AssetManager mAssetManager;
		TextureManager mTextureManager;
		FontManager mFontManager;

		Handle<Layer> mRootLayer;
		Handle<Layer> mGameLayer;
		Handle<Layer> mUILayer;
		Handle<Layer> mDebugLayer;

		vector<Colour> mColourStack;

		static GfxEngine *sMainGfxEngine;

		GfxEngine();
	};

}
}
