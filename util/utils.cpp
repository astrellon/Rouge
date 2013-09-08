#include "utils.h"

#include <fstream>
using namespace std;

#include <math/math.h>

namespace am {
namespace util {

	string Utils::toLowerCase(const char *input)
	{
		if (input == nullptr)
		{
			return string();
		}

		size_t len = strlen(input);
		char *buff = new char[len + 1];
		buff[len] = '\0';
		for (size_t i = 0; i < len; i++)
		{
			char ch = input[i];
			if (ch >= 'A' && ch <= 'Z')
			{
				ch |= 32;
			}
			buff[i] = ch;
		}
		string result = buff;
		delete buff;
		return result;
	}

	string Utils::toUpperCase(const char *input)
	{
		if (input == nullptr)
		{
			return string();
		}

		size_t len = strlen(input);
		char *buff = new char[len + 1];
		buff[len] = '\0';
		for (size_t i = 0; i < len; i++)
		{
			char ch = input[i];
			if (ch >= 'a' && ch <= 'z')
			{
				ch &= ~32;
			}
			buff[i] = ch;
		}
		string result = buff;
		delete buff;
		return result;
	}

	int Utils::countWordLetters( const string &str, int position, bool toRight )
	{
		int len = static_cast<int>(str.size());
		if (len == 0 || position < 0 || position > len)
		{
			return 0;
		}
		int pos = position;
		int whiteCount = 0;
		int notWhiteCount = 0;
		if (toRight && pos < len)
		{
			char ch = str[++pos];
			while (pos < len)
			{
				if (IS_WHITE(ch))
				{
					if (notWhiteCount > 0 && whiteCount == 0)
					{
						break;
					}
					whiteCount++;
				}
				else
				{
					if (whiteCount > 0 && notWhiteCount == 0)
					{
						break;
					}
					notWhiteCount++;
				}
				ch = str[++pos];
			}
		}
		else if (!toRight && pos > 0)
		{
			char ch = str[--pos];
			while (pos > 0)
			{
				if (IS_WHITE(ch))
				{
					if (notWhiteCount > 0 && whiteCount == 0)
					{
						break;
					}
					whiteCount++;
				}
				else
				{
					if (whiteCount > 0 && notWhiteCount == 0)
					{
						break;
					}
					notWhiteCount++;
				}
				ch = str[--pos];
			}
		}
		return pos - position;
	}

	float Utils::randf()
	{
		return static_cast<float>(::rand()) / static_cast<float>(RAND_MAX);
	}
	float Utils::randf(float lower, float upper)
	{
		float num = randf();
		float diff = upper - lower;
		return num * diff + lower;
	}
	int Utils::rand(int lower, int upper)
	{
		float num = randf();
		float diff = static_cast<float>(upper - lower);
		return math::round(num * diff) + lower;
	}
	void Utils::setRandSeed(unsigned int seed)
	{
		srand(seed);
	}

	bool Utils::fileExists(const char *filename)
	{
		ifstream stream(filename);
		return stream;
	}

}
}
