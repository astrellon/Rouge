#include "utils.h"

namespace am {
namespace util {

	string Utils::toLowerCase(const char *input)
	{
		if (input == NULL)
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
		if (input == NULL)
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

}
}
