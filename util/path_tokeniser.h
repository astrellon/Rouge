#pragma once

#include <cstring>

namespace am {
namespace util {

	// Simple path tokeniser, does not care about using
	// backslashes or forwardslashes.
	// 
	// Handler Windows local folders such as
	// C:\path/to\folder -> C: path to folder
	// 
	// Also Windows network paths
	// \\server\path/to\folder -> \\ server path to folder
	// 
	// And *nix style folder paths
	// /home/user\path/to\folder -> / home user path to folder
	//
	// And general relative paths
	// saves/game1/main.lua -> saves game1 main.lua
	//
	// Currently this should suffice, however there may be unforeseen
	// issuse with this tokeniser.

	class PathTokeniser {
	public:
		PathTokeniser(const char *input);
		~PathTokeniser();

		const char *nextToken();

	protected:

		int mIndex;
		const char *mInput;
		char mBuff[1024];
	};

}
}
