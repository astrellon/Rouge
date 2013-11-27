#pragma once

#include <string>

#include <util/tokeniser.h>
using namespace am::util;

namespace am {
namespace game {

	class Character;

	class StringPool {
	public:
		static std::string replace(const char *str);
		static std::string filterText(const std::string &str);

	protected:

		static std::string replaceCharacter(Tokeniser &tokeniser, Character *character);

	};

}
}
