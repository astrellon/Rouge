#include "sfx_source_area.h"

#include <algorithm>

#include "sfx_engine.h"
#include "sfx_isound.h"

namespace am {
namespace sfx {

	SourceArea::SourceArea() :
		ISource(),
		mWidth(1),
		mHeight(1)
#ifdef SOUND_3D
		,mDepth(1)
#endif
	{
	}
	SourceArea::SourceArea(ISound *sound) :
		ISource(sound),
		mWidth(1),
		mHeight(1)
#ifdef SOUND_3D
		,mDepth(1)
#endif
	{
		setSound(sound);
	}
	SourceArea::~SourceArea()
	{
		
	}

	void SourceArea::update()
	{
		if (mSound)
		{
			if (isOutOfRange(mClosestPosition.x, mClosestPosition.y))
			{
				stopOutOfRange();
			}
			ISource::update();
		}
	}
	void SourceArea::recalc()
	{
		SfxEngine *engine = SfxEngine::getEngine();
		mClosestPosition = mListener = engine->getListener().getPosition();
				
		if (mClosestPosition.x < mPosition.x)
		{
			mClosestPosition.x = mPosition.x;
		}
		if (mClosestPosition.x > mPosition.x + mWidth)
		{
			mClosestPosition.x = mPosition.x + mWidth;
		}

		if (mClosestPosition.y < mPosition.y)
		{
			mClosestPosition.y = mPosition.y;
		}
		if (mClosestPosition.y > mPosition.y + mHeight)
		{
			mClosestPosition.y = mPosition.y + mHeight;
		}

		alSource3f(mSource, AL_POSITION, mClosestPosition.x, mClosestPosition.y, 0.0f);
	}

	bool SourceArea::isOutOfRange() const
	{
		float gain = calcGain();
		return gain < 0.02f;
	}
	bool SourceArea::isOutOfRange(float posX, float posY) const
	{
		float dx = posX - mListener.x;
		float dy = posY - mListener.y;
		float dist = sqrtf(dx * dx + dy * dy);
		float gain = calcGain(dist);
		return gain < 0.02f;
	}

	float SourceArea::calcGain() const
	{
		float distance = static_cast<float>(SfxEngine::getEngine()->getListener().getPosition().distance(mClosestPosition));
		return calcGain(distance);
	}
	float SourceArea::calcGain(float distance) const
	{
		distance = std::max(distance, mReferenceDistance);
		float gain = mReferenceDistance / (mReferenceDistance + mRolloffFactor * (distance - mReferenceDistance));
		return gain * mGain;
	}


	void SourceArea::setWidth(float width)
	{
		if (width != mWidth)
		{
			mWidth = width;
		}
	}
	float SourceArea::getWidth() const
	{
		return mWidth;
	}
	void SourceArea::setHeight(float height)
	{
		if (height != mHeight)
		{
			mHeight = height;
		}
	}
	float SourceArea::getHeight() const
	{
		return mHeight;
	}
#ifdef SOUND_3D
	void SourceArea::setDepth(float depth)
	{
		if (depth != mDepth)
		{
			mDepth = depth;
		}
	}
	float SourceArea::getDepth() const
	{
		return mDepth;
	}

	void SourceArea::setSize(float width, float height, float depth)
	{
		if (mWidth != width || mHeight != height || mDepth != depth)
		{
			mWidth = width;
			mHeight = height;
			mDepth = depth;
		}
	}

	math::Vector4f SourceArea::getClosestPosition() const
	{
		return mClosestPosition;
	}
#else
	void SourceArea::setSize(float width, float height)
	{
		if (mWidth != width || mHeight != height)
		{
			mWidth = width;
			mHeight = height;
		}
	}

	math::Vector2f SourceArea::getClosestPosition() const
	{
		return mClosestPosition;
	}
#endif

}
}
