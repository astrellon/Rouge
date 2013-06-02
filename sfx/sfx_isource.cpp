#include "sfx_isource.h"

#include "sfx_engine.h"
#include "sfx_isound.h"

namespace am {
namespace sfx {

	ISource::ISource() :
		mSource(-1),
		mLooping(false),
		mGain(1.0f),
		mReferenceDistance(1.0f),
		mRolloffFactor(1.0f)
	{
	}
	ISource::ISource(ISound *sound) :
		mSource(-1),
		mLooping(false),
		mGain(1.0f),
		mReferenceDistance(1.0f),
		mRolloffFactor(1.0f)
	{
		setSound(sound);
	}
	ISource::~ISource()
	{
		releaseSource();
	}

	void ISource::play()
	{
		if (!mSound)
		{
			return;
		}
		if (isOutOfRange())
		{
			stopOutOfRange();
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
	void ISource::stop()
	{
		if (mPlaying && mSource == -1)
		{
			SfxEngine::getEngine()->removeInactiveSource(this);
		}
		mPlaying = false;
		releaseSource();
	}
	void ISource::stopOutOfRange()
	{
		// If we've been told to stop because we're out of range and
		// we're not a looping sound then just stop.
		// Otherwise we're still considered to be playing since we'll
		// still be going once we come back into range.
		mPlaying = mLooping;
		if (mPlaying)
		{
			SfxEngine::getEngine()->addInactiveSource(this);
		}
		releaseSource();
	}

	void ISource::update()
	{
		if (mSound)
		{
			if (isOutOfRange())
			{
				stopOutOfRange();
				return;
			}
			mSound->update();
			ALint status = getStatus();
			if (status != AL_PLAYING)
			{
				stop();
			}
		}
	}

	void ISource::setSound(ISound *sound)
	{
		releaseSource();
		mSound = sound;
	}
	ISound *ISource::getSound() const
	{
		return mSound;
	}

	void ISource::setLooping(bool value)
	{
		mLooping = value;
		if (mSource != -1)
		{
			alSourcei(mSource, AL_LOOPING, value);
		}
	}
	bool ISource::isLooping() const
	{
		return mLooping;
	}

	void ISource::setGain(float gain)
	{
		mGain = gain;
		if (mSource != -1)
		{
			alSourcef(mSource, AL_GAIN, gain);
		}
	}
	float ISource::getGain() const
	{
		return mGain;
	}

	void ISource::setReferenceDistance(float distance)
	{
		mReferenceDistance = distance;
		if (mSource != -1)
		{
			alSourcef(mSource, AL_REFERENCE_DISTANCE, distance);
		}
	}
	float ISource::getReferenceDistance() const
	{
		return mReferenceDistance;
	}

	void ISource::setRolloffFactor(float factor)
	{
		mRolloffFactor = factor;
		if (mSource != -1)
		{
			alSourcef(mSource, AL_ROLLOFF_FACTOR, factor);
		}
	}
	float ISource::getRolloffFactor() const
	{
		return mRolloffFactor;
	}

#ifdef SOUND_3D
	void ISource::setPosition(float x, float y, float z)
	{
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
		if (mSource != -1)
		{
			alSource3f(mSource, AL_POSITION, x, y, z);
		}
	}
	void ISource::setVelocity(float x, float y, float z)
	{
		mVelocity.x = x;
		mVelocity.y = y;
		mVelocity.z = z;
		if (mSource != -1)
		{
			alSource3f(mSource, AL_VELOCITY, x, y, z);
		}
	}
	Vector4f ISource::getPosition() const
	{
		return mPosition;
	}
	Vector4f ISource::getVelocity() const
	{
		return mVelocity;
	}
#else
	void ISource::setPosition(float x, float y)
	{
		mPosition.x = x;
		mPosition.y = y;
		if (mSource != -1)
		{
			alSource3f(mSource, AL_POSITION, x, y, 0.0f);
		}
	}
	void ISource::setVelocity(float x, float y)
	{
		mVelocity.x = x;
		mVelocity.y = y;
		if (mSource != -1)
		{
			alSource3f(mSource, AL_VELOCITY, x, y, 0.0f);
		}
	}
	Vector2f ISource::getPosition() const
	{
		return mPosition;
	}
	Vector2f ISource::getVelocity() const
	{
		return mVelocity;
	}
#endif

	ALint ISource::getStatus()
	{
		ALint result = AL_NONE;
		if (mSource != -1)
		{
			alGetSourcei( mSource, AL_SOURCE_STATE, &result);
		}
		return result;
	}

	ALuint ISource::getSourceId() const
	{
		return mSource;
	}

	void ISource::applyToSource()
	{
		if (mSource == -1)
		{
			return;
		}
		alSourcef(mSource, AL_GAIN, mGain);
		alSourcef(mSource, AL_REFERENCE_DISTANCE, mReferenceDistance);
		alSourcef(mSource, AL_ROLLOFF_FACTOR, mRolloffFactor);
		alSourcei(mSource, AL_LOOPING, mLooping);
#ifdef SOUND_3D
		alSource3f(mSource, AL_POSITION, mPosition.x, mPosition.y, mPosition.z);
		alSource3f(mSource, AL_VELOCITY, mVelocity.x, mVelocity.y, mVelocity.z);
#else
		alSource3f(mSource, AL_POSITION, mPosition.x, mPosition.y, 0.0f);
		alSource3f(mSource, AL_VELOCITY, mVelocity.x, mVelocity.y, 0.0f);
#endif
	}
	void ISource::releaseSource()
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
