#pragma once

#include <cstring>

namespace am {
namespace util {

	// A tokeniser for JSON, worked for most things except Unicode
	// and is currently unused since the move to Lua.

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
