#include "bounding_box.h"

#include <math/math.h>

namespace am {
namespace util {

	BoundingBox::BoundingBox() :
		mMin(0, 0, 0, 0),
		mMax(0, 0, 0, 0)
	{

	}
	BoundingBox::~BoundingBox()
	{

	}

	bool BoundingBox::isWithin(const math::Vector4f &point) const
	{
		return point.x >= mMin.x && point.y >= mMin.y &&
			point.x <= mMax.x && point.y <= mMax.y;
	}
	bool BoundingBox::isWithin(const int x, const int y) const
	{
		return x >= mMin.x && y >= mMin.y && 
			x <= mMax.x && y <= mMax.y;
	}
	bool BoundingBox::isWithin(const BoundingBox &box) const
	{
		return isWithin(box.mMin) && isWithin(box.mMax);
	}

	float BoundingBox::getWidth() const
	{
		return mMax.x - mMin.x;
	}
	float BoundingBox::getHeight() const
	{
		return mMax.y - mMin.y;
	}

	math::Vector4f BoundingBox::getMin() const
	{
		return mMin;
	}
	math::Vector4f BoundingBox::getMax() const
	{
		return mMax;
	}

	void BoundingBox::expandToInclude(const math::Vector4f &point)
	{
		mMin.x = math::minv(mMin.x, point.x);
		mMin.y = math::minv(mMin.y, point.y);

		mMax.x = math::maxv(mMax.x, point.x);
		mMax.y = math::maxv(mMax.y, point.y);
	}
	void BoundingBox::expandToInclude(const int x, const int y)
	{
		mMin.x = math::minv(mMin.x, static_cast<float>(x));
		mMin.y = math::minv(mMin.y, static_cast<float>(y));

		mMax.x = math::maxv(mMax.x, static_cast<float>(x));
		mMax.y = math::maxv(mMax.y, static_cast<float>(y));
	}
	void BoundingBox::expandToInclude(const BoundingBox &box)
	{
		expandToInclude(box.mMin);
		expandToInclude(box.mMax);
	}

}
}
