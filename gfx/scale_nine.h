#pragma once

namespace am {
namespace gfx {

	enum ScaleNineState {
		SCALE_NINE_NONE, SCALE_NINE_VERTICAL, SCALE_NINE_HORIZONTAL, SCALE_NINE_BOTH
	};

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

		static const char *getStateName(ScaleNineState state);
		static ScaleNineState getState(const char *stateName);

		bool atDefault() const;

	protected:

		float mLeft;
		float mRight;
		float mTop;
		float mBottom;
	};

}
}
