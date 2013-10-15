#pragma once

#include <ostream>
using namespace std;

#include <math/transform_lite.h>
using namespace am::math;

#include <ui/event_interface.h>
using namespace am::ui;

#include <util/colour.h>
using namespace am::util;

#include "gfx_effect.h"
#include "gfx_component.h"
#include "gfx_tooltip_component.h"

namespace am {
namespace gfx {

	class Layer;

	class Renderable : virtual public IManaged, public EventInterface {
	public:

		typedef vector<const Renderable *> RenderablePath;
		Renderable();
		Renderable(const Renderable &copy);
		virtual ~Renderable();

		virtual Renderable *clone() const;
		template <class T>
		T *clone() const
		{
			Renderable *cloned = clone();
			if (cloned)
			{
				T *result = dynamic_cast<T *>(cloned);
				if (result)
				{
					return result;
				}
			}
			return nullptr;
		}

		virtual void deinit();

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

		virtual void setGfxComponent(GfxComponent *comp);
		virtual GfxComponent *getGfxComponent() const;

		virtual am::math::TransformLite &getTransform();
		virtual void setPosition(float x, float y);
		virtual void setPosition(int x, int y);
		virtual float getPositionX() const;
		virtual float getPositionY() const;

		// Mostly for debug
		virtual void setName(const char *name);
		virtual string getName() const;

		virtual void setRenderColour(bool render);
		virtual bool getRenderColour() const;

		virtual void getRenderPath(RenderablePath &path) const;

		virtual void getScreenToLocal(const float &inX, const float &inY, float &outX, float &outY) const;
		virtual void getLocalToScreen(const float &inX, const float &inY, float &outX, float &outY) const;

		virtual void setTooltip(const char *tooltip);
		virtual const char *getTooltip();
		virtual void setDetailedTooltip(const char *tooltip);
		virtual const char *getDetailedTooltip();
		virtual void setTooltipEnabled(bool enabled);
		virtual bool isTooltipEnabled() const;

		friend std::ostream& operator<<(std::ostream&, const Renderable&);

		static void debugRenderPath(const RenderablePath &path, vector<string> &output);

	protected:

#ifdef _DEBUG
		string mDebugName;
		bool mRenderColour;
#endif

		float mWidth;
		float mHeight;
		bool mVisible;
		bool mInteractive;

		Handle<GfxComponent> mGfxComponent;
		Handle<TooltipComponent> mTooltip;
		Handle<Renderable> mTooltipGraphic;
		Layer *mParent;
		am::math::TransformLite mTransform;

		virtual void preRender(float dt);
		virtual void postRender(float dt);
	};

}
}
