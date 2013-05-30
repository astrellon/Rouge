#pragma once

#include <map>
#include <string>
using namespace std;

#include <al.h>

#include <base/imanaged.h>
using namespace am::base;

#include <math/vector.h>
using namespace am::math;

namespace am {
namespace sfx {

	class ISound;

	class Source : public IManaged {
	public:
		
		Source();
		Source(ISound *sound);
		~Source();

		void play();
		void stop();
		void stopOutOfRange();

		void update();

		void setSound(ISound *sound);
		ISound *getSound() const;

		void setLooping(bool value);
		bool isLooping() const;

		void setSourceRelative(bool value);
		bool isSourceRelative() const;

		void setGain(float gain);
		float getGain() const;

		void setReferenceDistance(float distance);
		float getReferenceDistance() const;

		void setRolloffFactor(float factor);
		float getRolloffFactor() const;

#ifdef SOUND_3D
		void setPosition(float x, float y, float z);
		void setVelocity(float x, float y, float z);
		Vector4f getPosition() const;
		Vector4f getVelocity() const;
#else
		void setPosition(float x, float y);
		void setVelocity(float x, float y);
		Vector2f getPosition() const;
		Vector2f getVelocity() const;
#endif

		float calcGain() const;
		bool isOutOfRange() const;

		ALint getStatus();
		ALuint getSourceId() const;

	protected:

		bool mLooping;
		bool mSourceRelative;
		bool mPlaying;
		ISound *mSound;
		ALuint mSource;
		float mGain;
		float mReferenceDistance;
		float mRolloffFactor;

#ifdef SOUND_3D
		Vector4f mPosition;
		Vector4f mVelocity;
#else
		Vector2f mPosition;
		Vector2f mVelocity;
#endif

		void applyToSource();
		void releaseSource();

	};

}
}
