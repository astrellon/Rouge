#pragma once

#include <sfx/sfx_isource.h>

#include <math/vector.h>

namespace am {
namespace sfx {

	class ISound;

	class SourceArea: public ISource 
	{
	public:
		
		SourceArea();
		SourceArea(ISound *sound);
		~SourceArea();

		virtual void update();
		virtual void recalc();

		virtual float calcGain() const;
		virtual float calcGain(float distance) const;
		virtual bool isOutOfRange() const;
		virtual bool isOutOfRange(float posX, float posY) const;

		virtual void setWidth(float width);
		virtual float getWidth() const;

		virtual void setHeight(float height);
		virtual float getHeight() const;

#ifdef SOUND_3D
		virtual void setDepth(float depth);
		virtual float getDepth() const;

		virtual void setSize(float width, float height, float depth);

		virtual math::Vector4f getClosestPosition() const;
#else
		virtual void setSize(float width, float height);

		virtual math::Vector2f getClosestPosition() const;
#endif

	protected:

		float mWidth;
		float mHeight;
#ifdef SOUND_3D
		float mDepth;
		math::Vector4f mClosestPosition;
		math::Vector4f mListener;
#else
		math::Vector2f mClosestPosition;
		math::Vector2f mListener;
#endif

	};

}
}
