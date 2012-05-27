#pragma once

#include <string>

using namespace std;

namespace am {
namespace gfx {

	class IAsset;
	class ITexture;
	class ISprite;
	class IFont;
	class ILayer;
	class ITextField;
	class ITextList;

	class IGfxEngine {
	public:
		virtual void init() = 0;
		virtual void deinit() = 0;
		virtual void setPerspective() = 0;
		virtual void setOrthographic() = 0;
	
		virtual void display(float dt) = 0;
		virtual void reshape(int width, int height) = 0;

		virtual IAsset *getAsset(const char *assetName) = 0;
		virtual ITexture *getTexture(const char *filename) = 0;
		virtual IFont *getFont(const char *fontName) = 0;

		virtual ISprite *getCursor() = 0;
		virtual void setCursor(ISprite *cursor) = 0;

		virtual ILayer *getRootLayer() = 0;

		virtual ISprite *createSprite(IAsset *asset) = 0;
		virtual ISprite *createSprite(const string &assetName) = 0;
		virtual ILayer *createLayer() = 0;
		virtual ITextField *createTextField() = 0;
		virtual ITextList *createTextList() = 0;

	};

}
}
