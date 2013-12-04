#include "sfx_source_point.h"

#include "sfx_engine.h"
#include "sfx_isound.h"

namespace am {
namespace sfx {

	SourcePoint::SourcePoint() :
		ISource(),
		mSourceRelative(false)
	{
	}
	SourcePoint::SourcePoint(ISound *sound) :
		ISource(sound),
		mSourceRelative(false)
	{
		setSound(sound);
	}
	SourcePoint::~SourcePoint()
	{
		
	}

	bool SourcePoint::isOutOfRange() const
	{
		float gain = calcGain();
		return gain < 0.02f;
	}

	void SourcePoint::setSourceRelative(bool value)
	{
		mSourceRelative = value;
		if (mSource != -1)
		{
			alSourcei(mSource, AL_SOURCE_RELATIVE, value);
		}
	}
	bool SourcePoint::isSourceRelative() const
	{
		return mSourceRelative;
	}

	float SourcePoint::calcGain() const
	{
		float distance = 0.0f;
		if (mSourceRelative)
		{
			distance = static_cast<float>(mPosition.length());
		}
		else
		{
			distance = static_cast<float>(SfxEngine::getEngine()->getListener().getPosition().distance(mPosition));
		}
		distance = max(distance, mReferenceDistance);
		float gain = mReferenceDistance / (mReferenceDistance + mRolloffFactor * (distance - mReferenceDistance));
		return gain * mGain;
	}

	void SourcePoint::applyToSource()
	{
		if (mSource == -1)
		{
			return;
		}
		ISource::applyToSource();
		alSourcei(mSource, AL_SOURCE_RELATIVE, mSourceRelative);
	}
	
}
}
