#pragma once

#include <cstring>

#define IS_WHITE(c)	c == ' ' || c == '\t' || c == '\r' || c == '\n'

namespace am {
namespace util {

class Tokeniser {
public:
	Tokeniser(const char *str);
	~Tokeniser();

	const char *nextToken();
	
private:
	const char *ctok;
	char tokBuff[512];
};

}
}

#ifdef _ANDROID
#	include "tokeniser.cpp"
#endif
