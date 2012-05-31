#pragma once

#include <vector>
#include <map>
#include <string>

using namespace std;

//#include "resource_manager.h"
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

		Layer *getRootLayer();

		void onMouseDown(int mouseButton, int x, int y);
		void onMouseMove(int mouseButton, int x, int y);
		void onMouseUp(int mouseButton, int x, int y);
		void onKeyDown(const bool *keys, int key);
		void onKeyUp(const bool *keys, int key);

	protected:
		
		int mScreenWidth;
		int mScreenHeight;

		Sprite *mCursor;

		//typedef am::util::ResourceManager<GlTexture *, string> TextureManager;
		typedef map<string, Asset *> AssetManager;
		typedef map<string, Texture *> TextureManager;
		typedef map<string, Font *> FontManager;

		AssetManager mAssetManager;
		TextureManager mTextureManager;
		FontManager mFontManager;

		Layer *mRootLayer;

		bool checkForMouseEvent(Renderable *target, const char *type, int mouseButton, int x, int y, int localX, int localY);
	};

}
}
