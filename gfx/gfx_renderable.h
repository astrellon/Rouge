#pragma once

#include "gfx_component.h"
#include "../transform.h"
#include "../event_manager.h"

namespace am {
namespace gfx {

	class Layer;

	class Renderable : public GfxComponent, public am::ui::EventManager {
	public:
		Renderable(GfxEngine *engine);
		virtual ~Renderable();

		virtual void render(float dt);

		virtual void setWidth(float width);
		virtual float getWidth() const;

		virtual void setHeight(float height);
		virtual float getHeight() const;

		virtual Layer *getParent() const;
		virtual void setParent(Layer *layer);

		virtual void setEnableInteractive(bool set);
		virtual bool getEnableInteractive() const;

		virtual void setVisible(bool visible);
		virtual bool getVisible() const;

		//virtual BoundingBox getBounds() = 0;
		virtual am::math::Transform &getTransform();
	protected:

		float mWidth;
		float mHeight;
		bool mVisible;
		Layer *mParent;
		bool mEnableInteractive;
		am::math::Transform mTransform;
	};

}
}
