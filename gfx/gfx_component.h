#pragma once

#include <vector>

#include <base/imanaged.h>
#include <base/handle.h>

#include <util/colour.h>
using am::util::Colour;

#include <gfx/gfx_effect.h>

namespace am {
namespace gfx {

	class GfxComponent : public base::IManaged {
	public:

		typedef std::vector< base::Handle<Effect> > EffectList;

		GfxComponent();
		GfxComponent(const GfxComponent &copy);
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
