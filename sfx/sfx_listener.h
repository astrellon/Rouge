#pragma once

#include <map>
#include <string>

#include <AL/al.h>

#include <base/imanaged.h>

#include <math/vector.h>

namespace am {
namespace sfx {

	class Listener : public base::IManaged
	{
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
		math::Vector4f getPosition() const;
		math::Vector4f getVelocity() const;
		math::Vector4f getOrientation() const;
#else
		void setPosition(float x, float y);
		void setVelocity(float x, float y);
		void setOrientation(float x, float y);
		math::Vector2f getPosition() const;
		math::Vector2f getVelocity() const;
		math::Vector2f getOrientation() const;
#endif

	protected:

		bool mEnabled;
		float mGain;

#ifdef SOUND_3D
		math::Vector4f mPosition;
		math::Vector4f mVelocity;
		math::Vector4f mOrientation;
#else
		math::Vector2f mPosition;
		math::Vector2f mVelocity;
		math::Vector2f mOrientation;
#endif

		void applyToListener();

	};

}
}
