#pragma once

#include <string>
#include <map>
#include <vector>
using namespace std;

//#include <base/handle.h>
//using namespace am::base;

//#include <game/character.h>
//#include <game/dialogue_choice.h>
//#include <game/dialogue_selector.h>

namespace am {
namespace game {

	class Character;

	class Dialogue {
	public:

		enum UnlockFlag {
			NONE, LOCKED
		};

		// Is a list of dialogue subjects
		typedef vector<string> DialogueChoices;

		Dialogue(const char *id, const char *text, const char *title = NULL, const char *subject = NULL, UnlockFlag unlock = NONE);
		~Dialogue();

		void setText(const char *text);
		const char *getText() const;

		void setTitle(const char *title);
		const char *getTitle() const;

		void setId(const char *id);
		const char *getId() const;

		void setSubject(const char *subject);
		const char *getSubject() const;

		void setUnlockFlag(UnlockFlag flag);
		UnlockFlag getUnlockFlag() const;

		static bool addDialogue(Dialogue *dialogue);
		static bool removeDialogue(const char *id);
		static void removeAllDialogue();
		static Dialogue *getDialogue(const char *id);

		static void getAvailableDIalogues(vector<Dialogue *> &result, const Character *talker, const Character *talkedTo);

	protected:

		string mText;
		string mId;
		string mTitle;
		string mSubject;
		UnlockFlag mUnlockFlag;

		typedef map<string, Dialogue *> DialogueMap;
		static DialogueMap sDialogueMap;

	};

}
}
