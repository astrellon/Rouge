#include "scale_nine.h"

#include <string>

#include <math/math.h>
using namespace am;

namespace am {
namespace gfx {

	ScaleNine::ScaleNine() :
		mLeft(0.0f),
		mRight(0.0f),
		mTop(0.0f),
		mBottom(0.0f)
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

	bool ScaleNine::atDefault() const
	{
		return math::abs(mLeft) < 0.000001f &&
			math::abs(mTop) < 0.000001f &&
			math::abs(mRight) < 0.000001f &&
			math::abs(mBottom) < 0.000001f;
	}

	const char *ScaleNine::getStateName(ScaleNineState state)
	{
		switch (state)
		{
		case SCALE_NINE_NONE:
			return "none";
		case SCALE_NINE_HORIZONTAL:
			return "horizontal";
		case SCALE_NINE_VERTICAL:
			return "vertical";
		case SCALE_NINE_BOTH:
			return "both";
		default:
			return "unknown";
		}
	}
	ScaleNineState ScaleNine::getState(const char *stateName)
	{
		if (stateName == nullptr || stateName[0] == '\0' || strcmp(stateName, "none") == 0)
		{
			return SCALE_NINE_NONE;
		}
		if (strcmp(stateName, "horizontal") == 0)
		{
			return SCALE_NINE_HORIZONTAL;
		}
		if (strcmp(stateName, "vertical") == 0)
		{
			return SCALE_NINE_VERTICAL;
		}
		if (strcmp(stateName, "both") == 0)
		{
			return SCALE_NINE_BOTH;
		}
		return SCALE_NINE_NONE;
	}

}
}
