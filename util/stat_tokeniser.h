#pragma once

#include <cstring>

namespace am {
namespace util {

	class StatTokeniser
	{
	public:
		StatTokeniser(const char *input, const char *delims);
		~StatTokeniser();

		const char *nextToken();

	protected:

		const char *mDelims;
		int mDelimsLength;
		const char *mInput;
		char mBuff[512];

		bool isDelim(char ch) const;
	};

}
}
