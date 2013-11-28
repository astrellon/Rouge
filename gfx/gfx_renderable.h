#pragma once

#include <ostream>

#include <math/transform_lite.h>

#include <ui/event_interface.h>

#include <util/colour.h>

#include "gfx_effect.h"
#include "gfx_component.h"

namespace am {
namespace gfx {

	class Layer;
	class Tooltip;

	class Renderable : virtual public base::IManaged, public ui::EventInterface {
	public:

		typedef std::vector<const Renderable *> RenderablePath;
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

		virtual math::TransformLite &getTransform();
		virtual void setPosition(float x, float y);
		virtual void setPosition(int x, int y);
		virtual float getPositionX() const;
		virtual float getPositionY() const;

		// Mostly for debug
		virtual void setName(const char *name);
		virtual std::string getName() const;

		virtual void setRenderColour(bool render);
		virtual bool getRenderColour() const;

		virtual void getRenderPath(RenderablePath &path) const;

		virtual void getScreenToLocal(const float &inX, const float &inY, float &outX, float &outY) const;
		virtual void getLocalToScreen(const float &inX, const float &inY, float &outX, float &outY) const;

		virtual void setTooltip(Tooltip *tooltip);
		virtual bool hasTooltip() const;
		virtual Tooltip *getTooltip();
		
		friend std::ostream& operator<<(std::ostream&, const Renderable&);

		static void debugRenderPath(const RenderablePath &path, std::vector<std::string> &output);

	protected:

#ifdef _DEBUG
		std::string mDebugName;
		bool mRenderColour;
#endif

		float mWidth;
		float mHeight;
		bool mVisible;
		bool mInteractive;

		base::Handle<GfxComponent> mGfxComponent;
		Tooltip *mTooltip;
		Layer *mParent;
		math::TransformLite mTransform;

		virtual void preRender(float dt);
		virtual void postRender(float dt);
	};

}
}
