#pragma once

#include <vector>
#include <map>
#include <string>

#include "../handle.h"

using namespace std;

#include "../matrix.h"

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
		// GlGfxEngine methods
		GfxEngine();
		~GfxEngine();

		Asset *getAsset(const char *assetName);
		Texture *getTexture(const char *textureName);
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

		void onKeyDown(const bool *keys, int key);
		void onKeyUp(const bool *keys, int key);

	protected:
		
		int mScreenWidth;
		int mScreenHeight;

		am::util::Handle<Sprite> mCursor;
		bool mHideCursor;

		//typedef am::util::ResourceManager<GlTexture *, string> TextureManager;
		typedef map<string, am::util::Handle<Asset> > AssetManager;
		typedef map<string, am::util::Handle<Texture> > TextureManager;
		typedef map<string, am::util::Handle<Font> > FontManager;

		AssetManager mAssetManager;
		TextureManager mTextureManager;
		FontManager mFontManager;

		am::util::Handle<Layer> mRootLayer;
		am::util::Handle<Layer> mGameLayer;
		am::util::Handle<Layer> mUILayer;
		am::util::Handle<Layer> mDebugLayer;
	};

}
}
