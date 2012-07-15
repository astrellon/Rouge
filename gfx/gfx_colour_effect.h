#pragma once

#include <util/colour.h>
using namespace am::util;

#include <gfx/gfx_effect.h>

namespace am {
namespace gfx {

	class ColourEffect: public Effect {
	public:
		ColourEffect();
		ColourEffect(float effectLength, const Colour &startColour, const Colour &endColour);
		~ColourEffect();

		virtual void setStartColour(const Colour &start);
		virtual Colour getStartColour() const;

		virtual void setEndColour(const Colour &end);
		virtual Colour getEndColour() const;

		virtual Colour getLerpedColour() const;

		virtual void update(float dt);
		virtual void applyToTarget(Renderable *target);

	protected:

		Colour mStartColour;
		Colour mEndColour;
		Colour mLerped;

	};

}
}
