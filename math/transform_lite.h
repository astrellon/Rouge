#pragma once

namespace am {
namespace math {

	class TransformLite
	{
	public:
		TransformLite();
		~TransformLite();

		float getX() const;
		float getY() const;
		void setX(float x);
		void setY(float y);
		void setXY(float x, float y);

		float getRotation() const;
		void setRotation(float rotation);

		void set(float x, float y, float rotation = 0.0f);

		void translate(float x, float y);
		void translateWorld(float x, float y);

		void apply();

	protected:
		float mX;
		float mY;
		float mRotation;
	};

}
}
