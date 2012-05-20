#pragma once

#include "igfx_component.h"
#include "transform.h"

namespace am {
namespace gfx {

	class IRenderable : public IGfxComponent {
	public:
		virtual am::math::Transform &getTransform() = 0;
		virtual void render() = 0;
	};

}
}
