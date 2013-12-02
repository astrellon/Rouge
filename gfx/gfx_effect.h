#pragma once

#include <base/imanaged.h>
#include <base/handle.h>

namespace am {
namespace gfx {

	class Renderable;

	class Effect: public base::IManaged 
	{
	public:
		Effect();
		Effect(float effectLength);
		~Effect();

		virtual void setCurrentTime(float time);
		virtual float getCurrentTime() const;
		virtual void reset();

		virtual void setEffectLength(float length);
		virtual float getEffectLength() const;

		virtual float getPercent() const;

		virtual void update(float dt);
		virtual void applyToTarget(Renderable *target) = 0;

	protected:

		float mEffectLength;
		float mEffectLengthResp;
		float mCurrentTime;

	};

}
}
