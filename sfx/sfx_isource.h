#pragma once

#include <map>
#include <string>

#include <AL/al.h>

#include <base/imanaged.h>

#include <math/vector.h>

namespace am {
namespace sfx {

	class ISound;

	class ISource : public base::IManaged 
	{
	public:
		
		ISource();
		ISource(ISound *sound);
		virtual ~ISource();

		virtual void play();
		virtual void stop();
		virtual void stopOutOfRange();

		virtual void update();
		virtual void recalc();

		virtual void setSound(ISound *sound);
		virtual ISound *getSound() const;

		virtual void setLooping(bool value);
		virtual bool isLooping() const;

		virtual void setGain(float gain);
		virtual float getGain() const;

		virtual void setReferenceDistance(float distance);
		virtual float getReferenceDistance() const;

		virtual void setRolloffFactor(float factor);
		virtual float getRolloffFactor() const;

		virtual float calcGain() const = 0;
		virtual bool isOutOfRange() const = 0;

		virtual ALint getStatus();
		virtual ALuint getSourceId() const;

#ifdef SOUND_3D
		virtual void setPosition(float x, float y, float z);
		virtual void setVelocity(float x, float y, float z);
		virtual math::Vector4f getPosition() const;
		virtual math::Vector4f getVelocity() const;
#else
		virtual void setPosition(float x, float y);
		virtual void setVelocity(float x, float y);
		virtual math::Vector2f getPosition() const;
		virtual math::Vector2f getVelocity() const;
#endif

	protected:

		bool mLooping;
		bool mPlaying;
		ISound *mSound;
		ALuint mSource;
		float mGain;
		float mReferenceDistance;
		float mRolloffFactor;

#ifdef SOUND_3D
		math::Vector4f mPosition;
		math::Vector4f mVelocity;
#else
		math::Vector2f mPosition;
		math::Vector2f mVelocity;
#endif

		virtual void applyToSource();
		virtual void releaseSource();

	};

}
}
