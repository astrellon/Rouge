#pragma once

#include "igfx_component.h"

namespace am {
namespace gfx {

	class IRenderable : public IGfxComponent {
	public:
		// TODO: Change to using proper transform based system for positioning.
		virtual void render(float x, float y) = 0;
	};

}
}
