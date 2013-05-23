#pragma once

#include <map>
#include <string>
using namespace std;

#include <al.h>

namespace am {
namespace sfx {

	class ISound;

	typedef struct _Vector3 {
		float x;
		float y;
		float z;

		_Vector3();
		_Vector3(float x, float y, float z);
	} Vector3;

	class Source {
	public:
		
		Source();
		Source(ISound *sound);
		~Source();

		void play();
		void stop();

		void update();

		void setSound(ISound *sound);
		ISound *getSound() const;

		void setLooping(bool value);
		bool isLooping() const;

		void setPriority(unsigned int priority);
		unsigned int getPriority() const;

		void setGain(float gain);
		float getGain() const;

		void setReferenceDistance(float distance);
		float getReferenceDistance() const;

		void setRolloffFactor(float factor);
		float getRolloffFactor() const;

		void setPosition(float x, float y, float z);
		void setVelocity(float x, float y, float z);

		ALint getStatus();
		ALuint getSourceId() const;

	protected:

		bool mLooping;
		unsigned int mPriority;
		ISound *mSound;
		ALuint mSource;
		float mGain;
		float mReferenceDistance;
		float mRolloffFactor;

		Vector3 mPosition;
		Vector3 mVelocity;

		void applyToSource();
		void releaseSource();

	};

}
}
