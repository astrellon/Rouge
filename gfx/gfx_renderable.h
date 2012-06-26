#pragma once

#include <ostream>
using namespace std;

#include "gfx_component.h"
#include "../transform_lite.h"
#include "../event_manager.h"
#include "../colour.h"
using namespace am::util;

namespace am {
namespace gfx {

	class Layer;

	class Renderable : public IManaged, public am::ui::EventManager {
	public:
		Renderable();
		virtual ~Renderable();

		virtual void render(float dt);
		
		virtual void setWidth(float width);
		virtual float getWidth();

		virtual void setHeight(float height);
		virtual float getHeight();

		virtual Layer *getParent() const;
		virtual void setParent(Layer *layer);

		virtual void setInteractive(bool set);
		virtual bool isInteractive() const;

		virtual void setVisible(bool visible);
		virtual bool isVisible() const;

		virtual Colour &getColour();
		virtual void setColour(const Colour &colour);
		virtual void setColour(float red, float green, float blue);
		virtual void setColour(float red, float green, float blue, float alpha);
		virtual void setAlpha(float alpha);
		virtual float getAlpha() const;

		//virtual BoundingBox getBounds() = 0;
		virtual am::math::TransformLite &getTransform();
		virtual void setPosition(float x, float y);
		virtual void setPosition(int x, int y);
		virtual float getPositionX() const;
		virtual float getPositionY() const;

		virtual void setName(const char *name);
		virtual string getName() const;

		friend std::ostream& operator<<(std::ostream&, const Renderable&);

	protected:

		string mName;

		float mWidth;
		float mHeight;
		bool mVisible;
		bool mInteractive;

		Layer *mParent;
		Colour mColour;
		am::math::TransformLite mTransform;

		virtual void preRender(float dt);
		virtual void postRender(float dt);
	};

}
}
