#pragma once

#include <ostream>
using namespace std;

#include <math/transform_lite.h>
using namespace am::math;

#include <ui/event_manager.h>
using namespace am::ui;

#include <util/colour.h>
using namespace am::util;

#include "gfx_component.h"
#include "gfx_effect.h"

namespace am {
namespace gfx {

	class Layer;

	class Renderable : public IManaged, public EventManager {
	public:
		Renderable();
		virtual ~Renderable();

		virtual void render(float dt);
		
		virtual void setSize(float width, float height);

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

		virtual void addEffect(Effect *effect);
		virtual void removeEffect(Effect *effect);
		virtual void clearAllEffects();

		// Mostly for debug
		virtual void setName(const char *name);
		virtual string getName() const;

		virtual void setRenderColour(bool render);
		virtual bool getRenderColour() const;

		virtual void getScreenToLocal(const float &inX, const float &inY, float &outX, float &outY) const;
		virtual void getLocalToScreen(const float &inX, const float &inY, float &outX, float &outY) const;

		friend std::ostream& operator<<(std::ostream&, const Renderable&);

	protected:

#ifdef _DEBUG
		string mDebugName;
		bool mRenderColour;
#endif

		float mWidth;
		float mHeight;
		bool mVisible;
		bool mInteractive;

		Layer *mParent;
		Colour mColour;
		am::math::TransformLite mTransform;

		typedef vector< Handle<Effect> > EffectList;
		EffectList mEffects;

		virtual void preRender(float dt);
		virtual void postRender(float dt);
	};

}
}
