#pragma once

#include <string>
#include <map>
#include <vector>
using namespace std;

#include <base/handle.h>
using namespace am::base;

#include <game/character.h>
#include <game/dialogue_choice.h>

namespace am {
namespace game {

	class DialogueSelector {
	public:

		DialogueSelector();
		~DialogueSelector();

	protected:

		string mGameObjId;

		typedef map<string, string> Attributes;
		Attributes mAttributes;

	};

}
}
