#pragma once

#include <base/handle.h>
using namespace am::base;

#include <util/colour.h>
using namespace am::util;

#include <gfx/gfx_font.h>

namespace am {
namespace gfx {

	class TextStyle {
	public:

		enum TextStylePop {
			NONE, COLOUR, FONT
		};

		TextStyle();
		//TextStyle(Font *font);
		TextStyle(const Colour &colour, int position);
		//TextStyle(Font *font, const Colour &colour);
		TextStyle(TextStylePop pop, int position);
		~TextStyle();

		void setColour(const Colour &colour);
		Colour getColour() const;
		void removeColour();
		bool hasColour() const;

		//void setFont(Font *font);
		//Font *getFont();

		void setTextStylePop(TextStylePop pop);
		TextStylePop getTextStylePop() const;

		void setPosition(int position);
		int getPosition() const;

	protected:

		int mPosition;
		TextStylePop mPop;
		Colour mColour;
		//Handle<Font> mFont;
	};

}
}
