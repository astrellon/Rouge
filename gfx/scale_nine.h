#pragma once

namespace am {
namespace gfx {

	class ScaleNine {
	public:
		ScaleNine();
		ScaleNine(float left, float right, float top, float bottom);
		~ScaleNine();

		void setLeft(float left);
		void setRight(float right);
		void setTop(float top);
		void setBottom(float bottom);
		void setInnerBounds(float left, float right, float top, float bottom);

		float getLeft() const;
		float getRight() const;
		float getTop() const;
		float getBottom() const;

	protected:

		float mLeft;
		float mRight;
		float mTop;
		float mBottom;
	};

}
}
