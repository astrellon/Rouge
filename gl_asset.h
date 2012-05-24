#pragma once

#include "iasset.h"
#include "igl_gfx_component.h"
#include "texture_window.h"

#include <string>

using namespace std;

namespace am {
namespace gfx {

	class GlTexture;
	class GlGfxEngine;

	class GlAsset : public IAsset, public IGlGfxComponent {
	public:
		// GlAsset methods
		~GlAsset();

		void setGlTexture(const GlTexture *texture);
		const GlTexture *getGlTexture() const;

		// IGfxAsset methods
		virtual const char *getName() const;

		virtual void setTexture(const ITexture *texture);
		virtual const ITexture *getTexture() const;

		virtual void setTextureWindow(const TextureWindow &window);
		virtual const TextureWindow &getTextureWindow() const;

		virtual int loadDef(JsonValue value);

		// IGfxComponent methods
		virtual IGfxEngine *getGfxEngine();
		// IGlGfxComponent methods
		virtual GlGfxEngine *getGlGfxEngine();

		friend class GlGfxEngine;

	protected:

		GlAsset(GlGfxEngine *engine, const char *name);

		string mName;
		const GlTexture *mTexture;
		TextureWindow mWindow;
		GlGfxEngine *mGfxEngine;
	};

}
}
