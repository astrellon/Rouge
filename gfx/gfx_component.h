#pragma once

#include "../imanaged.h"

namespace am {
namespace gfx {

	class GfxEngine;

	class GfxComponent : public am::util::IManaged {
	public:
		GfxComponent(GfxEngine *engine);
		virtual ~GfxComponent();

		virtual GfxEngine *getGfxEngine();

	protected:

		GfxEngine *mGfxEngine;
		
	};

}
}
