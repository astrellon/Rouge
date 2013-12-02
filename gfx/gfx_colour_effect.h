#pragma once

#include <util/colour.h>

#include <gfx/gfx_effect.h>

namespace am {
namespace gfx {

	class ColourEffect: public Effect 
	{
	public:
		ColourEffect();
		ColourEffect(float effectLength, const util::Colour &startColour, const util::Colour &endColour);
		~ColourEffect();

		virtual void setStartColour(const util::Colour &start);
		virtual util::Colour getStartColour() const;

		virtual void setEndColour(const util::Colour &end);
		virtual util::Colour getEndColour() const;

		virtual util::Colour getLerpedColour() const;

		virtual void update(float dt);
		virtual void applyToTarget(Renderable *target);

	protected:

		util::Colour mStartColour;
		util::Colour mEndColour;
		util::Colour mLerped;

	};

}
}
