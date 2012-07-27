#include "colour.h"

#include <gl.h>

#include <log/logger.h>

#include <sstream>
using namespace std;

#include <util/json_value.h>
#include <util/utils.h>

namespace am {
namespace util {

	std::ostream& operator<<(std::ostream &stream, const Colour &rhs)
	{
		stream << "[" << rhs.getRed() << ", " << rhs.getGreen() << ", " << rhs.getBlue() << ", " << rhs.getAlpha() << "]";
		return stream;
	}

	Colour::ColourMap Colour::sColourMap;

	Colour::Colour() :
		mRed(0.0f),
		mGreen(0.0f),
		mBlue(0.0f),
		mAlpha(1.0f)
	{
	}
	Colour::Colour(const char *name)
	{
		getNamedColour(name, *this);
	}
	Colour::Colour(float red, float green, float blue, float alpha) :
		mRed(red),
		mGreen(green),
		mBlue(blue),
		mAlpha(alpha)
	{
	}
	Colour::Colour(const Colour &rhs) :
		mRed(rhs.mRed),
		mGreen(rhs.mGreen),
		mBlue(rhs.mBlue),
		mAlpha(rhs.mAlpha)
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

	Colour Colour::lerp(const Colour &rhs, float dt) const
	{
		Colour c(*this);
		if (rhs.mRed != mRed)
		{
			c.mRed = mRed + (rhs.mRed - mRed) * dt;
		}
		if (rhs.mGreen != mGreen)
		{
			c.mGreen = mGreen + (rhs.mGreen - mGreen) * dt;
		}
		if (rhs.mBlue != mBlue)
		{
			c.mBlue = mBlue + (rhs.mBlue - mBlue) * dt;
		}
		if (rhs.mAlpha != mAlpha)
		{
			c.mAlpha = mAlpha + (rhs.mAlpha - mAlpha) * dt;
		}
		return c;
	}
	void Colour::parseFromString(const char *str)
	{
		if (str == NULL || str[0] == '\0')
		{
			return;
		}
		if (str[0] == '#' || str[0] == 'x' || str[0] == 'X')
		{
			unsigned int value = 0;
			bool parsed = Utils::fromString<unsigned int>(value, str + 1, std::hex);
			if (parsed)
			{
				parseFromUint(value);
			}
		}
		else
		{
			Tokeniser tokeniser(str);
			const char *token = tokeniser.nextToken();
			float value = 0.0f;
			if (token != NULL)
			{
				bool parsed = Utils::fromString(value, token);
				if (parsed)
				{
					setRed(value);
				}
				else
				{
					getNamedColour(token, *this);
				}
			}

			token = tokeniser.nextToken();
			if (token != NULL)
			{
				bool parsed = Utils::fromString(value, token);
				if (parsed)
				{
					setGreen(value);
				}
			}

			token = tokeniser.nextToken();
			if (token != NULL)
			{
				bool parsed = Utils::fromString(value, token);
				if (parsed)
				{
					setBlue(value);
				}
			}

			token = tokeniser.nextToken();
			if (token != NULL)
			{
				bool parsed = Utils::fromString(value, token);
				if (parsed)
				{
					setAlpha(value);
				}
				else
				{
					setAlpha(1.0f);
				}
			}
		}
	}
	void Colour::parseFromUint(unsigned int value)
	{
		float red = static_cast<float>((value >> 24) & 0xFF);
		float green = static_cast<float>((value >> 16) & 0xFF);
		float blue = static_cast<float>((value >> 8) & 0xFF);
		float alpha = static_cast<float>(value & 0xFF);

		setRed(red / 255.0f);
		setGreen(green / 255.0f);
		setBlue(blue / 255.0f);
		setAlpha(alpha / 255.0f);
	}

	void Colour::parseFromTokeniser(TextTokeniser &tokeniser)
	{
		const char *token = tokeniser.nextToken();
		if (!token || token[0] == '>')
		{
			return;
		}
		float value = 0;
		bool parsed = Utils::fromString<float>(value, token);
		if (parsed)
		{
			setRed(value);
		}
		else
		{
			string lower = Utils::toLowerCase(token);
			getNamedColour(lower, *this);
			return;
		}

		token = tokeniser.nextToken();
		if (!token || token[0] == '>')
		{
			return;
		}
		parsed = Utils::fromString<float>(value, token);
		if (parsed)
		{
			setGreen(value);
		}
				
		token = tokeniser.nextToken();
		if (!token || token[0] == '>')
		{
			return;
		}
		parsed = Utils::fromString<float>(value, token);
		if (parsed)
		{
			setBlue(value);
		}
	}

	bool Colour::getNamedColour(const char *name, Colour &result)
	{
		ColourMap::iterator iter = sColourMap.find(string(name));
		if (iter != sColourMap.end())
		{
			result = iter->second;
			return true;
		}
		return false;
	}
	bool Colour::getNamedColour(const string &name, Colour &result)
	{
		ColourMap::iterator iter = sColourMap.find(name);
		if (iter != sColourMap.end())
		{
			result = iter->second;
			return true;
		}
		return false;
	}
	void Colour::addNamedColour(const char *name, const Colour &colour)
	{
		sColourMap[string(name)] = colour;
	}
	void Colour::addNamedColour(const string &name, const Colour &colour)
	{
		sColourMap[name] = colour;
	}
	void Colour::removeNamedColour(const char *name)
	{
		ColourMap::iterator iter = sColourMap.find(string(name));
		if (iter != sColourMap.end())
		{
			sColourMap.erase(iter);
		}
	}
	void Colour::removeNamedColour(const string &name)
	{
		ColourMap::iterator iter = sColourMap.find(name);
		if (iter != sColourMap.end())
		{
			sColourMap.erase(iter);
		}
	}

	void Colour::addStandardNamedColours(const char *filename)
	{
		if (filename != NULL)
		{
			JsonValue loaded = JsonValue::import_from_file(filename);
			if (loaded.getType() == JV_OBJ)
			{
				JsonObject *obj = loaded.getObj();
				JsonObject::iterator iter;
				for (iter = obj->begin(); iter != obj->end(); ++iter)
				{
					Colour c;
					bool parsed = false;
					if (iter->second.getType() == JV_STR)
					{
						c.parseFromString(iter->second.getCStr());
						parsed = true;
					}
					else if (iter->second.getType() == JV_INT)
					{
						c.parseFromUint(iter->second.getInt());
						parsed = true;
					}

					if (parsed)
					{
						string lowerName = Utils::toLowerCase(iter->first.c_str());
						addNamedColour(lowerName, c);
					}
				}
				return;
			}
			stringstream errss;
			errss << "Failed to loaded standard colours from '" << filename << "', file returned a " << loaded.getTypeName();
			am_log("COLOUR", errss);
		}
		addNamedColour("white", Colour(1, 1, 1));
		addNamedColour("black", Colour(0, 0, 0));
		addNamedColour("red", Colour(1, 0, 0));
		addNamedColour("green", Colour(0, 1, 0));
		addNamedColour("blue", Colour(0, 0, 1));
	}

}
}
