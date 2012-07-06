#pragma once

#include <base/handle.h>
using namespace am::base;

#include <vector>
using namespace std;

#include "gfx_renderable.h"

namespace am {
namespace gfx {

	class Layer : public Renderable {
	public:
		// GlLayer methods
		Layer();
		~Layer();

		void addChild(Renderable *child);
		void addChild(Renderable *child, int index);
		void removeChild(int index);
		void removeChild(Renderable *child);

		void clear();

		bool hasChild(Renderable *child) const;
		bool hasChild(Handle<Renderable> child) const;
		int getNumChildren() const;
		Renderable *getChildAt(int index) const;

		// Renderable methods
		virtual void render(float dt);

	protected:

		typedef vector<Handle<Renderable> > ChildList;
		ChildList mChildren;

		ChildList::const_iterator findChild(Renderable *child) const;
	};

}
}
