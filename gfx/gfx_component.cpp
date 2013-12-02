#include "gfx_component.h"

#include "gfx_engine.h"

namespace am {
namespace gfx {

	GfxComponent::GfxComponent() :
		mColour(1.0f, 1.0f, 1.0f, 1.0f)
	{
	}
	GfxComponent::GfxComponent(const GfxComponent &copy) :
		mColour(copy.mColour)
	{
	}
	GfxComponent::~GfxComponent()
	{
	}

	util::Colour &GfxComponent::getColour()
	{
		return mColour;
	}
	void GfxComponent::setColour(const util::Colour &colour)
	{
		mColour = colour;
	}
	void GfxComponent::setColour(float red, float green, float blue)
	{
		mColour.setColour(red, green, blue);
	}
	void GfxComponent::setColour(float red, float green, float blue, float alpha)
	{
		mColour.setColour(red, green, blue, alpha);
	}

	void GfxComponent::setAlpha(float alpha)
	{
		mColour.setAlpha(alpha);
	}
	float GfxComponent::getAlpha() const
	{
		return mColour.getAlpha();
	}

	void GfxComponent::addEffect(Effect *effect)
	{
		mEffects.push_back(effect);
	}
	void GfxComponent::removeEffect(Effect *effect)
	{
		for (size_t i = 0; i < mEffects.size(); i++)
		{
			if (mEffects[i].get() == effect)
			{
				mEffects.erase(mEffects.begin() + i);
			}
		}
	}
	void GfxComponent::clearAllEffects()
	{
		mEffects.clear();
	}
	GfxComponent::EffectList &GfxComponent::getEffects()
	{
		return mEffects;
	}

}
}
