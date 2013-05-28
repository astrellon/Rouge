#include "sfx_source.h"

#include "sfx_engine.h"
#include "sfx_isound.h"

namespace am {
namespace sfx {

	Source::Source() :
		mSource(-1),
		mLooping(false),
		mGain(1.0f),
		mReferenceDistance(1.0f),
		mRolloffFactor(1.0f),
		//mPriority(1),
		mSourceRelative(false)
	{
	}
	Source::Source(ISound *sound) :
		mSource(-1),
		mLooping(false),
		mGain(1.0f),
		mReferenceDistance(1.0f),
		mRolloffFactor(1.0f),
		//mPriority(1),
		mSourceRelative(false)
	{
		setSound(sound);
	}
	Source::~Source()
	{
		releaseSource();
	}

	void Source::play()
	{
		if (!mSound)
		{
			return;
		}
		bool result = SfxEngine::getEngine()->getSource(mSource, this);
		if (!result)
		{
			return;
		}
		mPlaying = true;
		applyToSource();
		mSound->attachSource(mSource);
 		alSourcePlay(mSource);
	}
	void Source::stop()
	{
		mPlaying = false;
		releaseSource();
	}
	void Source::stopOutOfRange()
	{
		// If we've been told to stop because we're out of range and
		// we're not a looping sound then just stop.
		// Otherwise we're still considered to be playing since we'll
		// still be going once we come back into range.
		mPlaying = mLooping;
		if (mPlaying)
		{
			//SfxEngine::getEngine()->add
		}
		releaseSource();
	}

	void Source::update()
	{
		if (mSound)
		{
			float gain = calcGain();
			if (gain < 0.001f)
			{
				stopOutOfRange();
			}
			mSound->update();
			ALint status = getStatus();
			if (status != AL_PLAYING)
			{
				stop();
			}
		}
	}

	void Source::setSound(ISound *sound)
	{
		releaseSource();
		mSound = sound;
	}
	ISound *Source::getSound() const
	{
		return mSound;
	}

	void Source::setLooping(bool value)
	{
		mLooping = value;
		if (mSource != -1)
		{
			alSourcei(mSource, AL_LOOPING, value);
		}
	}
	bool Source::isLooping() const
	{
		return mLooping;
	}

	void Source::setSourceRelative(bool value)
	{
		mSourceRelative = value;
		if (mSource != -1)
		{
			alSourcei(mSource, AL_SOURCE_RELATIVE, value);
		}
	}
	bool Source::isSourceRelative() const
	{
		return mSourceRelative;
	}

	void Source::setGain(float gain)
	{
		mGain = gain;
		if (mSource != -1)
		{
			alSourcef(mSource, AL_GAIN, gain);
		}
	}
	float Source::getGain() const
	{
		return mGain;
	}

	void Source::setReferenceDistance(float distance)
	{
		mReferenceDistance = distance;
		if (mSource != -1)
		{
			alSourcef(mSource, AL_REFERENCE_DISTANCE, distance);
		}
	}
	float Source::getReferenceDistance() const
	{
		return mReferenceDistance;
	}

	void Source::setRolloffFactor(float factor)
	{
		mRolloffFactor = factor;
		if (mSource != -1)
		{
			alSourcef(mSource, AL_ROLLOFF_FACTOR, factor);
		}
	}
	float Source::getRolloffFactor() const
	{
		return mRolloffFactor;
	}

	/*void Source::setPriority(unsigned int priority)
	{
		mPriority = priority;
	}
	unsigned int Source::getPriority() const
	{
		return mPriority;
	}*/

#ifdef SOUND_3D
	void Source::setPosition(float x, float y, float z)
	{
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
		if (mSource != -1)
		{
			alSource3f(mSource, AL_POSITION, x, y, z);
		}
	}
	void Source::setVelocity(float x, float y, float z)
	{
		mVelocity.x = x;
		mVelocity.y = y;
		mVelocity.z = z;
		if (mSource != -1)
		{
			alSource3f(mSource, AL_VELOCITY, x, y, z);
		}
	}
	Vector4f Source::getPosition() const
	{
		return mPosition;
	}
	Vector4f Source::getVelocity() const
	{
		return mVelocity;
	}
#else
	void Source::setPosition(float x, float y)
	{
		mPosition.x = x;
		mPosition.y = y;
		if (mSource != -1)
		{
			alSource3f(mSource, AL_POSITION, x, y, 0.0f);
		}
	}
	void Source::setVelocity(float x, float y)
	{
		mVelocity.x = x;
		mVelocity.y = y;
		if (mSource != -1)
		{
			alSource3f(mSource, AL_VELOCITY, x, y, 0.0f);
		}
	}
	Vector2f Source::getPosition() const
	{
		return mPosition;
	}
	Vector2f Source::getVelocity() const
	{
		return mVelocity;
	}
#endif

	float Source::calcGain() const
	{
		float distance = SfxEngine::getEngine()->getListener().getPosition().distance(mPosition);
		distance = max(distance, mReferenceDistance);
		float gain = mReferenceDistance / (mReferenceDistance + mRolloffFactor * (distance - mReferenceDistance));
		return gain * mGain;
	}

	ALint Source::getStatus()
	{
		ALint result = AL_NONE;
		if (mSource != -1)
		{
			alGetSourcei( mSource, AL_SOURCE_STATE, &result);
		}
		return result;
	}

	ALuint Source::getSourceId() const
	{
		return mSource;
	}

	void Source::applyToSource()
	{
		if (mSource == -1)
		{
			return;
		}
		alSourcef(mSource, AL_GAIN, mGain);
		alSourcef(mSource, AL_REFERENCE_DISTANCE, mReferenceDistance);
		alSourcef(mSource, AL_ROLLOFF_FACTOR, mRolloffFactor);
		alSourcei(mSource, AL_LOOPING, mLooping);
		alSourcei(mSource, AL_SOURCE_RELATIVE, mSourceRelative);
#ifdef SOUND_3D
		alSource3f(mSource, AL_POSITION, mPosition.x, mPosition.y, mPosition.z);
		alSource3f(mSource, AL_VELOCITY, mVelocity.x, mVelocity.y, mVelocity.z);
#else
		alSource3f(mSource, AL_POSITION, mPosition.x, mPosition.y, 0.0f);
		alSource3f(mSource, AL_VELOCITY, mVelocity.x, mVelocity.y, 0.0f);
#endif
	}
	void Source::releaseSource()
	{
		if (mSource != -1)
		{
			alSourceStop(mSource);
			if (mSound && mSource != -1)
			{
				mSound->detachSource(mSource);
			}
			SfxEngine::getEngine()->releaseSource(mSource);
			mSource = -1;
		}
	}
}
}
