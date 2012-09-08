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

	class Dialogue {
	public:

		// Is a list of dialogue subjects
		typedef vector<string> DialogueChoices;

		Dialogue(const char *id, const char *text, const char *title = NULL, const char *subject = NULL);
		~Dialogue();

		//const char *getId() const;

		void setText(const char *text);
		const char *getText() const;

		void setTitle(const char *title);
		const char *getTitle() const;

		//void setTalker(Character *talker);
		//Character *getTalker() const;

		void setId(const char *id);
		const char *getId() const;

		void setSubject(const char *subject);
		const char *getSubject() const;

		//DialogueChoices &getChoices();

		static bool addDialogue(Dialogue *dialogue);
		static bool removeDialogue(const char *id);
		static Dialogue *getDialogue(const char *id);

		static bool setSubjectFlag(const char *subject, int flag);
		static bool getSubjectFlag(const char *subject, int &flag);

	protected:

		string mText;
		string mId;
		string mTitle;
		string mSubject;
		//DialogueChoices mChoices;

		//Handle<Character> mTalker;

		typedef map<string, Dialogue *> DialogueMap;
		static DialogueMap sDialogueMap;

		typedef map<string, int> SubjectMap;
		static SubjectMap sSubjectMap;

		//typedef pair<DialogueSelector, Dialogue *> DialogueSelectorPair;
		//typedef vector< DialogueSelectorPair > DialogueSelectorList;
		//typedef map<string, Dialogue *> DialogueMap;
		//static DialogueSelectorList sDialogueList;

	};

}
}
