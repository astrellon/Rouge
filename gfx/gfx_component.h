#pragma once

namespace am {
namespace gfx {

	class GfxEngine;

	class GfxComponent {
	public:
		GfxComponent(GfxEngine *engine);
		virtual ~GfxComponent();

		virtual GfxEngine *getGfxEngine();

	protected:

		GfxEngine *mGfxEngine;
		
	};

}
}
