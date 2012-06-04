#pragma once

namespace am {
namespace util {

	class Colour {
	public:
		Colour();
		Colour(float red, float green, float blue, float alpha = 1.0f);
		~Colour();

		float getRed() const;
		float getGreen() const;
		float getBlue() const;
		float getAlpha() const;

		void setRed(float red);
		void setGreen(float green);
		void setBlue(float blue);
		void setAlpha(float alpha);

		void setColour(float red, float green, float blue);
		void setColour(float red, float green, float blue, float alpha);

		void applyColour() const;

	protected:

		float mRed;
		float mGreen;
		float mBlue;
		float mAlpha;
	};

}
}
