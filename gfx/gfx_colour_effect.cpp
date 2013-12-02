#include "gfx_colour_effect.h"

#include "gfx_renderable.h"

namespace am {
namespace gfx {

	ColourEffect::ColourEffect() :
		Effect()
	{
	}
	ColourEffect::ColourEffect(float effectLength, const util::Colour &startColour, const util::Colour &endColour) :
		Effect(effectLength),
		mStartColour(startColour),
		mEndColour(endColour)
	{
	}
	ColourEffect::~ColourEffect()
	{
	}

	void ColourEffect::setStartColour(const util::Colour &start)
	{
		mStartColour = start;
	}
	util::Colour ColourEffect::getStartColour() const
	{
		return mStartColour;
	}

	void ColourEffect::setEndColour(const util::Colour &end)
	{
		mEndColour = end;
	}
	util::Colour ColourEffect::getEndColour() const
	{
		return mEndColour;
	}

	util::Colour ColourEffect::getLerpedColour() const
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
