#pragma once

#include <string>
#include "json_value.h"
#include "texture_window.h"

#include "igfx_component.h"

using namespace std;
using namespace am::util;

namespace am {
namespace gfx {

	class ITexture;
	class IGfxEngine;

	class IFont : public IGfxComponent {
	public:
		
		virtual ITexture *getTexture() = 0;
		virtual void setTexture(ITexture *texture) = 0;

		virtual string getName() const = 0;

		virtual int loadDef(JsonValue value) = 0;

		virtual void getTextureWindow(char ch, TextureWindow &render) = 0;

		virtual bool isLoaded() const = 0;
		virtual bool isFixedWidth() const = 0;

		virtual float getCharHeight() const = 0;
		virtual float getFixedCharWidth() const = 0;

		virtual void setKerning(float kerning) = 0;
		virtual float getKerning() const = 0;

		virtual void setLeading(float leading) = 0;
		virtual float getLeading() const = 0;

		virtual void setSpaceWidth(float width) = 0;
		virtual float getSpaceWidth() const = 0;

		virtual void setTabWidth(float width) = 0;
		virtual float getTabWidth() const = 0;

	};

}
}
