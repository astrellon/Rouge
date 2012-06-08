#include "scale_nine.h"

namespace am {
namespace gfx {

	ScaleNine::ScaleNine() :
		mLeft(0.0f),
		mRight(1.0f),
		mTop(0.0f),
		mBottom(1.0f)
	{
	}
	ScaleNine::ScaleNine(float left, float right, float top, float bottom) :
		mLeft(left),
		mRight(right),
		mTop(top),
		mBottom(bottom)
	{
	}
	ScaleNine::~ScaleNine()
	{
	}

	void ScaleNine::setLeft(float left)
	{
		mLeft = left;
	}
	void ScaleNine::setRight(float right)
	{
		mRight = right;
	}
	void ScaleNine::setTop(float top)
	{
		mTop = top;
	}
	void ScaleNine::setBottom(float bottom)
	{
		mBottom = bottom;
	}
	void ScaleNine::setInnerBounds(float left, float right, float top, float bottom)
	{
		mLeft = left;
		mRight = right;
		mTop = top;
		mBottom = bottom;
	}

	float ScaleNine::getLeft() const
	{
		return mLeft;
	}
	float ScaleNine::getRight() const
	{
		return mRight;
	}
	float ScaleNine::getTop() const
	{
		return mTop;
	}
	float ScaleNine::getBottom() const
	{
		return mBottom;
	}

}
}
