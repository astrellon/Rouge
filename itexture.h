#pragma once

#include "igfx_component.h"

namespace am {
namespace gfx {

	class ITexture : public IGfxComponent {
		
		virtual void destroy() = 0;
		
		virtual bool isLoaded() const = 0;

		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;
		virtual int getBytesPerPixel() const = 0;
		virtual int getGlFormat() const = 0;

	};

}
}
