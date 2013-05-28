#include "sfx_listener.h"

namespace am {
namespace sfx {

	Listener::Listener() :
		mGain(1.0f),
		mEnabled(false)
	{
	}
	Listener::~Listener()
	{
	}

	void Listener::setGain(float gain)
	{
		mGain = gain;
		if (mEnabled)
		{
			alListenerf(AL_GAIN, gain);
		}
	}
	float Listener::getGain() const
	{
		return mGain;
	}

	void Listener::setEnabled(bool enabled)
	{
		mEnabled = enabled;
		if (enabled)
		{
			applyToListener();
		}
	}
	bool Listener::isEnabled() const
	{
		return mEnabled;
	}

#ifdef SOUND_3D
	void Listener::setPosition(float x, float y, float z)
	{
		mPosition.x = x;
		mPosition.y = y;
		mPosition.z = z;
		if (mEnabled)
		{
			alListener3f(AL_POSITION, x, y, z);
		}
	}
	void Listener::setVelocity(float x, float y, float z)
	{
		mVelocity.x = x;
		mVelocity.y = y;
		mVelocity.z = z;
		if (mEnabled)
		{
			alListener3f(AL_VELOCITY, x, y, z);
		}
	}
	void Listener::setOrientation(float x, float y, float z)
	{
		mOrientation.x = x;
		mOrientation.y = y;
		mOrientation.z = z;
		if (mEnabled)
		{
			alListener3f(AL_ORIENTATION, x, y, z);
		}
	}
	Vector4f Listener::getPosition() const
	{
		return mPosition;
	}
	Vector4f Listener::getVelocity() const
	{
		return mVelocity;
	}
	Vector4f Listener::getOrientation() const
	{
		return mOrientation;
	}
#else
	void Listener::setPosition(float x, float y)
	{
		mPosition.x = x;
		mPosition.y = y;
		if (mEnabled)
		{
			alListener3f(AL_POSITION, x, y, 0.0f);
		}
	}
	void Listener::setVelocity(float x, float y)
	{
		mVelocity.x = x;
		mVelocity.y = y;
		if (mEnabled)
		{
			alListener3f(AL_VELOCITY, x, y, 0.0f);
		}
	}
	void Listener::setOrientation(float x, float y)
	{
		mOrientation.x = x;
		mOrientation.y = y;
		if (mEnabled)
		{
			alListener3f(AL_ORIENTATION, x, y, 0.0f);
		}
	}
	Vector2f Listener::getPosition() const
	{
		return mPosition;
	}
	Vector2f Listener::getVelocity() const
	{
		return mVelocity;
	}
	Vector2f Listener::getOrientation() const
	{
		return mOrientation;
	}
#endif
	
	void Listener::applyToListener()
	{
		if (!mEnabled)
		{
			return;
		}
		alListenerf(AL_GAIN, mGain);
#ifdef SOUND_3D
		alListener3f(AL_POSITION, mPosition.x, mPosition.y, mPosition.z);
		alListener3f(AL_VELOCITY, mVelocity.x, mVelocity.y, mVelocity.z);
		alListener3f(AL_ORIENTATION, mOrientation.x, mOrientation.y, mOrientation.z);
#else
		alListener3f(AL_POSITION, mPosition.x, mPosition.y, 0.0f);
		alListener3f(AL_VELOCITY, mVelocity.x, mVelocity.y, 0.0f);
		alListener3f(AL_ORIENTATION, mOrientation.x, mOrientation.y, 0.0f);
#endif
	}

}
}
