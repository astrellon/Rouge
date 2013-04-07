#pragma once

#include <cstring>

namespace am {
namespace util {

	// This tokeniser is for tokenising game text.
	// This basically splits based on XML like nodes.
	// For parsing strings like
	// "Hello there <name character='main'>Player Name</name>, how are you?"

	class TextTokeniser {
	public:
		TextTokeniser(const char *str);
		~TextTokeniser();

		const char *nextToken();
		const char *currentToken() const;
	
	private:

		char mInFormat;
		const char *ctok;
		char tokBuff[2048];
	};

}
}

