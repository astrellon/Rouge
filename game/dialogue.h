#pragma once

#include <string>
#include <map>
#include <vector>
using namespace std;

#include <game/dialogue_choice.h>

namespace am {
namespace game {

	class Dialogue {
	public:

		typedef vector<DialogueChoice> DialogueChoices;

		Dialogue(const char *id, const char *text);
		~Dialogue();

		const char *getId() const;

		void setText(const char *text);
		const char *getText() const;

		DialogueChoices &getChoices();

		static bool addDialogue(Dialogue *dialogue);
		static bool removeDialogue(const char *id);
		static Dialogue *getDialogue(const char *id);

	protected:

		string mText;
		string mId;
		DialogueChoices mChoices;

		typedef map<string, Dialogue *> DialogueMap;
		static DialogueMap sDialogueMap;

	};

}
}
