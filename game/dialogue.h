#pragma once

#include <string>
#include <vector>
using namespace std;

#include <game/dialogue_choice.h>

namespace am {
namespace game {

	class Dialogue {
	public:

		typedef vector<DialogueChoice> DialogueChoices;

		Dialogue(const char *text);
		~Dialogue();

		void setText(const char *text);
		const char *getText() const;

		DialogueChoices &getChoices();

	protected:

		string mText;
		DialogueChoices mChoices;

	};

}
}
