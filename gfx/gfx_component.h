#pragma once

#include <vector>
using std::vector;

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <util/colour.h>
using am::util::Colour;

#include <gfx/gfx_effect.h>

namespace am {
namespace gfx {

	class GfxComponent : public IManaged {
	public:

		typedef vector< Handle<Effect> > EffectList;

		GfxComponent();
		~GfxComponent();

		virtual Colour &getColour();
		virtual void setColour(const Colour &colour);
		virtual void setColour(float red, float green, float blue);
		virtual void setColour(float red, float green, float blue, float alpha);
		virtual void setAlpha(float alpha);
		virtual float getAlpha() const;

		virtual void addEffect(Effect *effect);
		virtual void removeEffect(Effect *effect);
		virtual void clearAllEffects();
		virtual EffectList &getEffects();

	protected:

		Colour mColour;
		EffectList mEffects;
		
	};

}
}
