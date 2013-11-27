#pragma once

#include <string>
#include <map>
#include <vector>

#include <base/handle.h>

#include <game/character.h>
#include <game/dialogue_choice.h>

namespace am {
namespace game {

	class DialogueSelector {
	public:

		DialogueSelector();
		~DialogueSelector();

	protected:

		std::string mGameObjId;

		typedef std::map<std::string, std::string> Attributes;
		Attributes mAttributes;

	};

}
}
