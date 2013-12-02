#pragma once

#include <vector>

#include <base/imanaged.h>
#include <base/handle.h>

#include <util/colour.h>

#include <gfx/gfx_effect.h>

namespace am {
namespace gfx {

	class GfxComponent : public base::IManaged 
	{
	public:

		typedef std::vector< base::Handle<Effect> > EffectList;

		GfxComponent();
		GfxComponent(const GfxComponent &copy);
		~GfxComponent();

		virtual util::Colour &getColour();
		virtual void setColour(const util::Colour &colour);
		virtual void setColour(float red, float green, float blue);
		virtual void setColour(float red, float green, float blue, float alpha);
		virtual void setAlpha(float alpha);
		virtual float getAlpha() const;

		virtual void addEffect(Effect *effect);
		virtual void removeEffect(Effect *effect);
		virtual void clearAllEffects();
		virtual EffectList &getEffects();

	protected:

		util::Colour mColour;
		EffectList mEffects;
		
	};

}
}
