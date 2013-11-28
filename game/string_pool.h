#pragma once

#include <string>

#include <util/tokeniser.h>

namespace am {
namespace game {

	class Character;

	class StringPool {
	public:
		static std::string replace(const char *str);
		static std::string filterText(const std::string &str);

	protected:

		static std::string replaceCharacter(util::Tokeniser &tokeniser, Character *character);

	};

}
}
