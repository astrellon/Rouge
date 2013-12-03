#pragma once

#include <math/vector.h>

namespace am {
namespace util {

	class BoundingBox 
	{
	public:
		BoundingBox();
		~BoundingBox();

		bool isWithin(const math::Vector4f &point) const;
		bool isWithin(const int x, const int y) const;
		bool isWithin(const BoundingBox &box) const;

		float getWidth() const;
		float getHeight() const;

		math::Vector4f getMin() const;
		math::Vector4f getMax() const;

		void expandToInclude(const math::Vector4f &point);
		void expandToInclude(const int x, const int y);
		void expandToInclude(const BoundingBox &box);

	protected:
		math::Vector4f mMin;
		math::Vector4f mMax;
	};

}
}
