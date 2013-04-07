#include "path_tokeniser.h"

#include <util/utils.h>

namespace am {
namespace util {

	PathTokeniser::PathTokeniser(const char *input) :
		mInput(input),
		mIndex(0)
	{
	}
	PathTokeniser::~PathTokeniser()
	{

	}

	const char *PathTokeniser::nextToken()
	{
		if (mInput[0] == '\0')
		{
			return NULL;
		}

		int pos = 0;
		bool inPath = false;
		int outPos = 0;
		char ch = mInput[0];
		while (ch != '\0')
		{
			// Special case for *nix paths.
			if (mIndex == 0 && ch == '/')
			{
				mBuff[0] = '/';
				outPos = pos = 1;
				break;
			}
			// Special case for dealing with Windows network paths.
			if (mIndex == 1 && ch == '\\' && mInput[pos - 1] == '\\')
			{
				mBuff[0] = '\\';
				mBuff[1] = '\\';
				outPos = pos = 2;
				break;
			}
			if (ch == '/' || ch == '\\')
			{
				if (inPath)
				{
					break;
				}
			}
			else
			{
				inPath = true;
			}
			if (inPath)
			{
				mBuff[outPos++] = ch;
			}
			ch = mInput[++pos];
			++mIndex;
		}
		mBuff[outPos] = '\0';
		mInput += pos;
		// If nothing was put into the buffer and we're at the end
		// of the input, then return NULL.
		if (outPos == 0 && ch == '\0')
		{
			return NULL;
		}
		return mBuff;
	}

}
}
