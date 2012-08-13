#include "transform_lite.h"

#ifndef UNIT_TEST
#	include <gl.h>
#endif

namespace am {
namespace math {

	TransformLite::TransformLite() :
		mX(0.0f),
		mY(0.0f),
		mRotation(0.0f)
	{
	}

	TransformLite::~TransformLite()
	{
	}

	float TransformLite::getX() const
	{
		return mX;
	}
	float TransformLite::getY() const
	{
		return mY;
	}
	void TransformLite::setX(float x)
	{
		mX = x;
	}
	void TransformLite::setY(float y)
	{
		mY = y;
	}
	void TransformLite::setXY(float x, float y)
	{
		mX = x;
		mY = y;
	}

	float TransformLite::getRotation() const
	{
		return mRotation;
	}
	void TransformLite::setRotation(float rotation)
	{
		mRotation = rotation;
	}

	void TransformLite::set(float x, float y, float rotation)
	{
		mX = x;
		mY = y;
		mRotation = rotation;
	}

	void TransformLite::translate(float x, float y)
	{
		// TODO
	}
	void TransformLite::translateWorld(float x, float y)
	{
		mX += x;
		mY += y;
	}

	void TransformLite::apply()
	{
		glTranslatef(mX, mY, 0);
		if (mRotation != 0.0f)
		{
			glRotatef(mRotation, 0.0f, 1.0f, 0.0f);
		}
	}
}
}
