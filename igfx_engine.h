#pragma once

namespace am {
namespace gfx {

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

		virtual int loadFontDefinitions(const char *filename) = 0;

		virtual ITexture *loadTexture(const char *filename) = 0;
		virtual IFont *getFont(const char *fontName) = 0;

	};

}
}
