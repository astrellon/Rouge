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

	class Dialogue {
	public:

		typedef vector<DialogueChoice> DialogueChoices;

		Dialogue(const char *id, const char *text);
		~Dialogue();

		const char *getId() const;

		void setText(const char *text);
		const char *getText() const;

		void setTalker(Character *talker);
		Character *getTalker() const;

		DialogueChoices &getChoices();

		static bool addDialogue(Dialogue *dialogue);
		static bool removeDialogue(const char *id);
		static Dialogue *getDialogue(const char *id);

	protected:

		string mText;
		string mId;
		DialogueChoices mChoices;

		Handle<Character> mTalker;

		typedef map<string, Dialogue *> DialogueMap;
		static DialogueMap sDialogueMap;

	};

}
}
