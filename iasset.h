#pragma once

#include "igfx_component.h"
#include "texture_window.h"

#include "json_value.h"

using namespace am::util;

namespace am {
namespace gfx {

	class ITexture;

	class IAsset : public IGfxComponent {
	public:
		virtual const char *getName() const = 0;

		virtual void setTexture(const ITexture *texture) = 0;
		virtual const ITexture *getTexture() const = 0;

		virtual void setTextureWindow(const TextureWindow &window) = 0;
		virtual const TextureWindow &getTextureWindow() const = 0;

		virtual int loadDef(JsonValue value) = 0;
		
	};

}
}
