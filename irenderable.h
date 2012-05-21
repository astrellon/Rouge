#pragma once

#include "igfx_component.h"
#include "transform.h"

namespace am {
namespace gfx {

	class IRenderable : public IGfxComponent {
	public:
		virtual am::math::Transform &getTransform() = 0;
		virtual void render(float dt) = 0;

		virtual void setWidth(float width) = 0;
		virtual float getWidth() const = 0;

		virtual void setHeight(float height) = 0;
		virtual float getHeight() const = 0;
	};

}
}
