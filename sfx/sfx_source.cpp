#include "sfx_source.h"

#include "sfx_engine.h"
#include "sfx_isound.h"

namespace am {
namespace sfx {

	_Vector3::_Vector3() :
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{
	}
	_Vector3::_Vector3(float x, float y, float z) :
		x(x),
		y(y),
		z(z)
	{
	}

	Source::Source() :
		mSource(-1),
		mLooping(false),
		mGain(1.0f),
		mReferenceDistance(1.0f),
		mRolloffFactor(1.0f),
		mPriority(1)
	{
	}
	Source::Source(ISound *sound) :
		mSource(-1),
		mLooping(false),
		mGain(1.0f),
		mReferenceDistance(1.0f),
		mRolloffFactor(1.0f),
		mPriority(1)
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
		ALenum error = alGetError();
		applyToSource();
		ALenum error1 = alGetError();
		mSound->attachSource(mSource);
		ALenum error2 = alGetError();
 		alSourcePlay(mSource);
		ALenum error3 = alGetError();
	}
	void Source::stop()
	{
		releaseSource();
	}

	void Source::update()
	{
		if (mSound)
		{
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

	void Source::setPriority(unsigned int priority)
	{
		mPriority = priority;
	}
	unsigned int Source::getPriority() const
	{
		return mPriority;
	}

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
		alSource3f(mSource, AL_POSITION, mPosition.x, mPosition.y, mPosition.z);
		alSource3f(mSource, AL_VELOCITY, mVelocity.x, mVelocity.y, mVelocity.z);
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
