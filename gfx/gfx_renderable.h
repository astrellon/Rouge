#pragma once

#include "gfx_component.h"
#include "../transform.h"
#include "../event_manager.h"

#include "../colour.h"

using namespace am::util;

namespace am {
namespace gfx {

	class Layer;

	class Renderable : public GfxComponent, public am::ui::EventManager {
	public:
		Renderable(GfxEngine *engine);
		virtual ~Renderable();

		virtual void render(float dt);
		
		virtual void setWidth(float width);
		virtual float getWidth();

		virtual void setHeight(float height);
		virtual float getHeight();

		virtual Layer *getParent() const;
		virtual void setParent(Layer *layer);

		virtual void setEnableInteractive(bool set);
		virtual bool getEnableInteractive() const;

		virtual void setVisible(bool visible);
		virtual bool getVisible() const;

		virtual Colour &getColour();
		virtual void setColour(const Colour &colour);
		virtual void setColour(float red, float green, float blue);
		virtual void setColour(float red, float green, float blue, float alpha);

		//virtual BoundingBox getBounds() = 0;
		virtual am::math::Transform &getTransform();
		virtual void setPosition(float x, float y);
		virtual void setPosition(int x, int y);

	protected:

		float mWidth;
		float mHeight;
		bool mVisible;
		Layer *mParent;
		Colour mColour;
		bool mEnableInteractive;
		am::math::Transform mTransform;

		virtual void preRender(float dt);
		virtual void postRender(float dt);
	};

}
}
