#pragma once

#include "gfx_component.h"
#include "texture_window.h"
#include "../json_value.h"
#include <string>

using namespace std;
using namespace am::util;

namespace am {
namespace gfx {

	class Texture;
	class GfxEngine;

	class Asset : public GfxComponent {
	public:
		// GlAsset methods
		Asset(GfxEngine *engine, const char *name);
		~Asset();

		const char *getName() const;

		void setTexture(const Texture *texture);
		const Texture *getTexture() const;

		void setTextureWindow(const TextureWindow &window);
		const TextureWindow &getTextureWindow() const;

		int loadDef(JsonValue value);

	protected:

		string mName;
		const Texture *mTexture;
		TextureWindow mWindow;
	};

}
}
