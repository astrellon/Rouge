#pragma once

#include <math/vector.h>

using namespace am::math;

namespace am {
namespace util {

	class BoundingBox {
	public:
		BoundingBox();
		~BoundingBox();

		bool isWithin(const Vector4f &point) const;
		bool isWithin(const int x, const int y) const;
		bool isWithin(const BoundingBox &box) const;

		float getWidth() const;
		float getHeight() const;

		Vector4f getMin() const;
		Vector4f getMax() const;

		void expandToInclude(const Vector4f &point);
		void expandToInclude(const int x, const int y);
		void expandToInclude(const BoundingBox &box);

	protected:
		Vector4f mMin;
		Vector4f mMax;
	};

}
}
