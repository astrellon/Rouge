#pragma once

#include <base/imanaged.h>

namespace am {
namespace gfx {

	class GfxEngine;

	class GfxComponent : public am::base::IManaged {
	public:
		GfxComponent(GfxEngine *engine);
		virtual ~GfxComponent();

		virtual GfxEngine *getGfxEngine();

	protected:

		GfxEngine *mGfxEngine;
		
	};

}
}
