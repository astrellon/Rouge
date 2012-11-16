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
		static string replace(const char *str);
		static string filterText(const string &str);

	protected:

		static string replaceCharacter(Tokeniser &tokeniser, Character *character);

	};

}
}
