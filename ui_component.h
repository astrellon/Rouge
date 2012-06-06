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

		virtual void setParentAnchorX(AnchorX anchor);
		virtual void setParentAnchorY(AnchorY anchor);
		virtual void setParentAnchor(AnchorX anchorX, AnchorY anchorY);

		virtual AnchorX getParentAnchorX() const;
		virtual AnchorY getParentAnchorY() const;
		
		virtual void setOffsetX(float offset);
		virtual void setOffsetY(float offset);
		virtual void setOffset(float offsetX, float offsetY);

		virtual float getOffsetX() const;
		virtual float getOffsetY() const;

		virtual void setParentOffsetX(float offset);
		virtual void setParentOffsetY(float offset);
		virtual void setParentOffset(float offsetX, float offsetY);

		virtual float getParentOffsetX() const;
		virtual float getParentOffsetY() const;

		virtual float getParentWidth();
		virtual float getParentHeight();

		virtual void setEnabled(bool enabled);
		virtual bool isEnabled() const;

		virtual bool isInteractive() const;

	protected:
		AnchorX mAnchorX;
		AnchorY mAnchorY;

		AnchorX mParentAnchorX;
		AnchorY mParentAnchorY;

		float mOffsetX;
		float mOffsetY;

		float mParentOffsetX;
		float mParentOffsetY;

		bool mEnabled;

		virtual void preRender(float dt);
	};

}
}
