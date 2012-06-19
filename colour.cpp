#include "colour.h"

#include "gl.h"

namespace am {
namespace util {

	Colour::Colour() :
		mRed(0.0f),
		mGreen(0.0f),
		mBlue(0.0f),
		mAlpha(1.0f)
	{
	}
	Colour::Colour(float red, float green, float blue, float alpha) :
		mRed(red),
		mGreen(green),
		mBlue(blue),
		mAlpha(alpha)
	{
	}
	Colour::~Colour()
	{
	}

	float Colour::getRed() const
	{
		return mRed;
	}
	float Colour::getGreen() const
	{
		return mGreen;
	}
	float Colour::getBlue() const
	{
		return mBlue;
	}
	float Colour::getAlpha() const
	{
		return mAlpha;
	}

	void Colour::setRed(float red)
	{
		mRed = red;
	}
	void Colour::setGreen(float green)
	{
		mGreen = green;
	}
	void Colour::setBlue(float blue)
	{
		mBlue = blue;
	}
	void Colour::setAlpha(float alpha)
	{
		mAlpha = alpha;
	}

	void Colour::setColour(float red, float green, float blue)
	{
		mRed = red;
		mGreen = green;
		mBlue = blue;
	}
	void Colour::setColour(float red, float green, float blue, float alpha)
	{
		mRed = red;
		mGreen = green;
		mBlue = blue;
		mAlpha = alpha;
	}

	void Colour::multiply(const Colour &rhs)
	{
		mRed *= rhs.mRed;
		mGreen *= rhs.mGreen;
		mBlue *= rhs.mBlue;
		mAlpha *= rhs.mAlpha;
	}

	void Colour::applyColour() const
	{
		glColor4f(mRed, mGreen, mBlue, mAlpha);
	}

}
}
