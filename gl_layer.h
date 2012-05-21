#pragma once

#include "ilayer.h"
#include "igl_renderable.h"

#include <vector>

using namespace std;

namespace am {
namespace gfx {

	class GlGfxEngine;
	class IGlRenderable;

	class GlLayer : public ILayer, public IGlRenderable {
	public:
		// GlLayer methods
		GlLayer(GlGfxEngine *engine);
		~GlLayer();

		void addGlChild(IGlRenderable *child);
		void removeGlChild(IGlRenderable *child);
		bool hasGlChild(IGlRenderable *child) const;
		IGlRenderable *getGlChildAt(int index) const;

		// ILayer methods
		virtual void addChild(IRenderable *child);
		virtual void removeChild(int index);
		virtual void removeChild(IRenderable *child);

		virtual bool hasChild(IRenderable *child) const;
		virtual int getNumChildren() const;
		virtual IRenderable *getChildAt(int index) const;

		// IRenderable methods
		virtual void render(float dt);
		virtual am::math::Transform &getTransform();

		virtual void setWidth(float width);
		virtual float getWidth() const;

		virtual void setHeight(float height);
		virtual float getHeight() const;

		// IGfxComponent methods
		virtual IGfxEngine *getGfxEngine();
		// IGlGfxComponent methods
		virtual GlGfxEngine *getGlGfxEngine();

	protected:

		typedef vector<IGlRenderable *> ChildList;
		ChildList mChildren;

		float mWidth;
		float mHeight;
		am::math::Transform mTransform;
		GlGfxEngine *mGfxEngine;

		ChildList::const_iterator findChild(IGlRenderable *child) const;

	};

}
}
