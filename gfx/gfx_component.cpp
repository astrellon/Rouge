#include "gfx_component.h"

#include "gfx_engine.h"

namespace am {
namespace gfx {

	GfxComponent::GfxComponent(GfxEngine *engine) :
		mGfxEngine(engine)
	{
	}
	GfxComponent::~GfxComponent()
	{
	}

	GfxEngine *GfxComponent::getGfxEngine()
	{
		return mGfxEngine;
	}
}
}
