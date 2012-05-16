#pragma once

#include "igfx_component.h"

namespace am {
namespace gfx {

	class GlGfxEngine;

	class IGlGfxComponent : public IGfxComponent {
	public:

		virtual GlGfxEngine *getGlGfxEngine() = 0;
	};

}
}
