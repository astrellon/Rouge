#pragma once

#include <map>
#include <string>
#include <ostream>
using namespace std;

#include <util/text_tokeniser.h>

namespace am {
namespace util {

	class Colour {
	public:
		Colour();
		Colour(const char *name);
		Colour(float red, float green, float blue, float alpha = 1.0f);
		Colour(const Colour &rhs);
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

		void multiply(const Colour &rhs);

		void applyColour() const;

		void parseFromTokeniser(TextTokeniser &tokeniser);
		void parseFromString(const char *str);
		void parseFromUint(unsigned int value);

		Colour lerp(const Colour &rhs, float dt) const;

		static bool getNamedColour(const char *name, Colour &result);
		static bool getNamedColour(const string &name, Colour &result);
		static void addNamedColour(const char *name, const Colour &colour);
		static void addNamedColour(const string &name, const Colour &colour);
		static void removeNamedColour(const char *name);
		static void removeNamedColour(const string &name);

		static void addStandardNamedColours(const char *filename);
		static void addStandardNamedColoursLua(const char *filename);

		friend std::ostream& operator<<(std::ostream& stream, const Colour& rhs);

	protected:

		float mRed;
		float mGreen;
		float mBlue;
		float mAlpha;

		typedef map<string, Colour> ColourMap;
		static ColourMap sColourMap;
	};

}
}
