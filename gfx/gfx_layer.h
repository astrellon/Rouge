#pragma once

#include "gfx_renderable.h"
#include "../handle.h"
#include <vector>

using namespace std;

namespace am {
namespace gfx {

	class GfxEngine;

	class Layer : public Renderable {
	public:
		// GlLayer methods
		Layer(GfxEngine *engine);
		~Layer();

		void addChild(Renderable *child);
		void addChild(Renderable *child, int index);
		void removeChild(int index);
		void removeChild(Renderable *child);

		void clear();

		bool hasChild(Renderable *child) const;
		int getNumChildren() const;
		Renderable *getChildAt(int index) const;

		// Renderable methods
		virtual void render(float dt);

	protected:

		typedef vector<am::util::Handle<Renderable> > ChildList;
		ChildList mChildren;

		ChildList::const_iterator findChild(Renderable *child) const;
	};

}
}
