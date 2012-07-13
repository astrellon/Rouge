#include "gfx_text_style.h"

namespace am {
namespace gfx {

	TextStyle::TextStyle() :
		mColour(-1, -1, -1, -1),
		mPop(NONE),
		mPosition(0)
	{
	}
	TextStyle::TextStyle(const Colour &colour, int position) :
		mColour(colour),
		mPop(NONE),
		mPosition(position)
	{
	}
	TextStyle::TextStyle(TextStylePop pop, int position) :
		mColour(-1, -1, -1, -1),
		mPop(pop),
		mPosition(position)
	{
	}
	TextStyle::~TextStyle()
	{
	}

	void TextStyle::setColour(const Colour &colour)
	{
		mColour = colour;
	}
	Colour TextStyle::getColour() const
	{
		return mColour;
	}
	void TextStyle::removeColour()
	{
		mColour.setColour(-1, -1, -1, -1);
	}
	bool TextStyle::hasColour() const
	{
		return mColour.getRed() >= 0.0f;
	}

	void TextStyle::setTextStylePop(TextStylePop pop)
	{
		mPop = pop;
	}
	TextStyle::TextStylePop TextStyle::getTextStylePop() const
	{
		return mPop;
	}

	void TextStyle::setPosition(int position)
	{
		mPosition = position;
	}
	int TextStyle::getPosition() const
	{
		return mPosition;
	}

}
}
