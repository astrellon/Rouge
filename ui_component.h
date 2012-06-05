#pragma once

#include "gfx/gfx_layer.h"

using namespace am::gfx;

namespace am {
namespace gfx {
	class GfxEngine;
}

namespace ui {

	enum AnchorX {
		X_LEFT, X_CENTER, X_RIGHT
	};
	enum AnchorY {
		Y_TOP, Y_CENTER, Y_BOTTOM
	};

	class UIComponent : public Layer {
	public:
		UIComponent(GfxEngine *engine);
		~UIComponent();

		virtual void setAnchorX(AnchorX anchor);
		virtual void setAnchorY(AnchorY anchor);
		virtual void setAnchor(AnchorX anchorX, AnchorY anchorY);

		virtual AnchorX getAnchorX() const;
		virtual AnchorY getAnchorY() const;

		virtual void setOffsetX(float offset);
		virtual void setOffsetY(float offset);
		virtual void setOffset(float offsetX, float offsetY);

		virtual float getOffsetX() const;
		virtual float getOffsetY() const;

		virtual float getParentWidth();
		virtual float getParentHeight();

		// Renderable methods
		//virtual void render(float dt);

	protected:
		AnchorX mAnchorX;
		AnchorY mAnchorY;

		float mOffsetX;
		float mOffsetY;

		virtual void preRender(float dt);
	};

}
}
