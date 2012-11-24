#include "gfx_colour_effect.h"

#include "gfx_renderable.h"

namespace am {
namespace gfx {

	ColourEffect::ColourEffect() :
		Effect()
	{
	}
	ColourEffect::ColourEffect(float effectLength, const Colour &startColour, const Colour &endColour) :
		Effect(effectLength),
		mStartColour(startColour),
		mEndColour(endColour)
	{
	}
	ColourEffect::~ColourEffect()
	{
	}

	void ColourEffect::setStartColour(const Colour &start)
	{
		mStartColour = start;
	}
	Colour ColourEffect::getStartColour() const
	{
		return mStartColour;
	}

	void ColourEffect::setEndColour(const Colour &end)
	{
		mEndColour = end;
	}
	Colour ColourEffect::getEndColour() const
	{
		return mEndColour;
	}

	Colour ColourEffect::getLerpedColour() const
	{
		return mLerped;
	}

	void ColourEffect::update(float dt)
	{
		Effect::update(dt);
		
		mLerped = mStartColour.lerp(mEndColour, getPercent());
	}

	void ColourEffect::applyToTarget(Renderable *target)
	{
		if (target && target->getGfxComponent())
		{
			target->getGfxComponent()->setColour(mLerped);
		}
	}

}
}
