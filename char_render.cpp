#include "char_render.h"

namespace am {
namespace gfx {

	CharRender::CharRender()
	{
	}
	CharRender::~CharRender()
	{
	}

	float CharRender::getWidth() const
	{
		return mWidth;
	}
	float CharRender::getHeight() const
	{
		return mHeight;
	}

	float CharRender::getTopY() const
	{
		return mTopY;
	}
	float CharRender::getBottomY() const
	{
		return mBottomY;
	}

	float CharRender::getLeftX() const
	{
		return mLeftX;
	}
	float CharRender::getRightX() const
	{
		return mRightX;
	}
	void CharRender::setValues(float width, float height,
		float topY, float bottomY,
		float leftX, float rightX)
	{
		mWidth = width;
		mHeight = height;
		mTopY = topY;
		mBottomY = bottomY;
		mLeftX = leftX;
		mRightX = rightX;
	}
}
}
