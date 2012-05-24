#pragma once

namespace am {
namespace gfx {

	class IAsset;
	class ITexture;
	class IFont;

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

	};

}
}
