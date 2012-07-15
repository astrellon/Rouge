#include "gfx_effect.h"

namespace am {
namespace gfx {

	Effect::Effect() :
		mCurrentTime(0.0f),
		mEffectLength(1.0f),
		mEffectLengthResp(1.0f)
	{
	}
	Effect::Effect(float effectLength) :
		mCurrentTime(0.0f)
	{
		setEffectLength(effectLength);
	}
	Effect::~Effect()
	{
	}

	void Effect::setCurrentTime(float time)
	{
		if (time < 0.0f)
		{
			time = 0.0f;
		}
		if (time > mEffectLength)
		{
			time = mEffectLength;
		}
		mCurrentTime = time;
	}
	float Effect::getCurrentTime() const
	{
		return mCurrentTime;
	}
	void Effect::reset()
	{
		mCurrentTime = 0.0f;
	}

	void Effect::setEffectLength(float length)
	{
		if (length < 0.0f)
		{
			length = 0.0f;
		}
		mEffectLength = length;
		mEffectLengthResp = 1.0f / length;
		setCurrentTime(mCurrentTime);
	}
	float Effect::getEffectLength() const
	{
		return mEffectLength;
	}

	float Effect::getPercent() const
	{
		return mCurrentTime * mEffectLengthResp;
	}

	void Effect::update(float dt)
	{
		setCurrentTime(mCurrentTime + dt);
	}

}
}
