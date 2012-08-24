#pragma once

#include <string>
using namespace std;

#include <util/tokeniser.h>
using namespace am::util;

namespace am {
namespace game {

	class Character;

	class StringPool {
	public:
		//StringPool();
		//~StringPool();

		static string replace(const char *str);

	protected:

		static string replaceCharacter(Tokeniser &tokeniser, Character *character);

	};

}
}
