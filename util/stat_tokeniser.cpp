#include "stat_tokeniser.h"

#include <util/tokeniser.h>

namespace am {
namespace util {

	StatTokeniser::StatTokeniser(const char *input, const char *delims) :
		mInput(input),
		mDelims(delims)
	{
		mDelimsLength = strlen(delims);
	}
	StatTokeniser::~StatTokeniser()
	{

	}

	const char *StatTokeniser::nextToken()
	{
		if (mInput[0] == '\0')
		{
			return NULL;
		}

		int foundChars = 0;
		int pos = 0;
		int outPos = 0;
		char ch = mInput[0];
		while (ch != '\0')
		{
			bool isWhite = IS_WHITE(ch);
			if (isWhite)
			{
				if (foundChars > 0)
				{
					break;
				}
				ch = mInput[++pos];
				continue;
			}
			
			int currentChar = isDelim(ch) ? 2 : 1;
			if (foundChars == 0)
			{
				foundChars = currentChar;
			}
			else
			{
				if (foundChars != currentChar)
				{
					break;
				}
			}
			mBuff[outPos++] = ch;
			ch = mInput[++pos];
		}
		mBuff[outPos] = '\0';
		mInput += pos;
		return mBuff;
	}

	bool StatTokeniser::isDelim(char ch) const
	{
		for (int i = 0; i < mDelimsLength; i++)
		{
			if (ch == mDelims[i])
			{
				return true;
			}
		}
		return false;
	}

}
}
