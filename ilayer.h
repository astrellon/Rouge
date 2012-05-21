#pragma once

#include "irenderable.h"

namespace am {
namespace gfx {

	class ILayer : public IRenderable {
	public:
		virtual void addChild(IRenderable *child) = 0;
		virtual void removeChild(int index) = 0;
		virtual void removeChild(IRenderable *child) = 0;
		virtual bool hasChild(IRenderable *child) const = 0;
		virtual int getNumChildren() const = 0;
		virtual IRenderable *getChildAt(int index) const = 0;
		
	};

}
}
