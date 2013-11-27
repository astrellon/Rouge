#pragma once

#include <map>
#include <string>
using namespace std;

#include <AL/al.h>

#include <base/imanaged.h>
using namespace am::base;

#include <math/vector.h>
using namespace am::math;

namespace am {
namespace sfx {

	class Listener : public IManaged {
	public:
		
		Listener();
		~Listener();

		void setGain(float gain);
		float getGain() const;

		void setEnabled(bool enabled);
		bool isEnabled() const;

#ifdef SOUND_3D
		void setPosition(float x, float y, float z);
		void setVelocity(float x, float y, float z);
		void setOrientation(float x, float y, float z);
		Vector4f getPosition() const;
		Vector4f getVelocity() const;
		Vector4f getOrientation() const;
#else
		void setPosition(float x, float y);
		void setVelocity(float x, float y);
		void setOrientation(float x, float y);
		Vector2f getPosition() const;
		Vector2f getVelocity() const;
		Vector2f getOrientation() const;
#endif

	protected:

		bool mEnabled;
		float mGain;

#ifdef SOUND_3D
		Vector4f mPosition;
		Vector4f mVelocity;
		Vector4f mOrientation;
#else
		Vector2f mPosition;
		Vector2f mVelocity;
		Vector2f mOrientation;
#endif

		void applyToListener();

	};

}
}
