#include "bounding_box.h"

#define _min(x, y)	(x < y ? x : y)
#define _max(x, y)	(x > y ? x : y)

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
		mMin.x = _min(mMin.x, point.x);
		mMin.y = _min(mMin.y, point.y);

		mMax.x = _max(mMax.x, point.x);
		mMax.y = _max(mMax.y, point.y);
	}
	void BoundingBox::expandToInclude(const int x, const int y)
	{
		mMin.x = _min(mMin.x, x);
		mMin.y = _min(mMin.y, y);

		mMax.x = _max(mMax.x, x);
		mMax.y = _max(mMax.y, y);
	}
	void BoundingBox::expandToInclude(const BoundingBox &box)
	{
		expandToInclude(box.mMin);
		expandToInclude(box.mMax);
	}

}
}
