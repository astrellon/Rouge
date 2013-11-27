#include "colour.h"

#ifndef UNIT_TEST
#	include <gl.h>
#endif

#include <log/logger.h>

#include <sstream>

#include <util/utils.h>
#include <util/tokeniser.h>

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace util {

	const Colour Colour::WHITE(1.0f, 1.0f, 1.0f, 1.0f);

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
#ifndef UNIT_TEST
		glColor4f(mRed, mGreen, mBlue, mAlpha);
#endif
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
		if (str == nullptr || str[0] == '\0')
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
			if (token != nullptr)
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
			if (token != nullptr)
			{
				bool parsed = Utils::fromString(value, token);
				if (parsed)
				{
					setGreen(value);
				}
			}

			token = tokeniser.nextToken();
			if (token != nullptr)
			{
				bool parsed = Utils::fromString(value, token);
				if (parsed)
				{
					setBlue(value);
				}
			}

			token = tokeniser.nextToken();
			if (token != nullptr)
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
			else
			{
				setAlpha(1.0f);
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
			std::string lower = Utils::toLowerCase(token);
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
		ColourMap::iterator iter = sColourMap.find(std::string(name));
		if (iter != sColourMap.end())
		{
			result = iter->second;
			return true;
		}
		return false;
	}
	bool Colour::getNamedColour(const std::string &name, Colour &result)
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
		sColourMap[std::string(name)] = colour;
	}
	void Colour::addNamedColour(const std::string &name, const Colour &colour)
	{
		sColourMap[name] = colour;
	}
	void Colour::removeNamedColour(const char *name)
	{
		ColourMap::iterator iter = sColourMap.find(std::string(name));
		if (iter != sColourMap.end())
		{
			sColourMap.erase(iter);
		}
	}
	void Colour::removeNamedColour(const std::string &name)
	{
		ColourMap::iterator iter = sColourMap.find(name);
		if (iter != sColourMap.end())
		{
			sColourMap.erase(iter);
		}
	}

	void Colour::addStandardNamedColoursLua(const char *filename)
	{
		if (filename != nullptr)
		{
			LuaState lua(false);
			if (lua.loadFile(filename))
			{
				lua_getglobal(lua, "colours");
				if (lua_istable(lua, -1))
				{
					/* table is in the stack at index 't' */
					lua_pushnil(lua);  /* first key */
					while (lua_next(lua, -2) != 0) {
						/* uses 'key' (at index -2) and 'value' (at index -1) */
						if (lua_isstring(lua, -2))
						{
							Colour c;
							bool parsed = false;
							const char *name = lua_tostring(lua, -2);
							if (lua_isstring(lua, -1))
							{
								const char *value = lua_tostring(lua, -1);
								c.parseFromString(value);
								parsed = true;
							}
							else if (lua_isnumber(lua, -1))
							{
								int value = static_cast<unsigned int>(lua_tointeger(lua, -1));
								c.parseFromUint(value);
								parsed = true;
							}
							if (parsed)
							{
								std::string lowerName = Utils::toLowerCase(name);
								addNamedColour(lowerName, c);
							}
						}
						/* removes 'value'; keeps 'key' for next iteration */
						lua_pop(lua, 1);
					}
				}
			}
			lua.close();
		}
	}

	void Colour::removeAllColours()
	{
		sColourMap.clear();
	}

}
}
