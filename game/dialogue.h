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

	class GameObject;

	class Dialogue {
	public:

		enum UnlockFlag {
			UNLOCK_NONE, UNLOCK_LOCKED, UNLOCK_UNKNOWN
		};

		enum DialogueAction {
			ACTION_NONE, ACTION_SHOP, ACTION_CLOSE, ACTION_UNKNOWN
		};

		// Is a list of dialogue subjects
		//typedef vector<string> DialogueChoices;

		Dialogue(const char *id, const char *text, const char *title = NULL, const char *subject = NULL, UnlockFlag unlock = UNLOCK_NONE, DialogueAction action = ACTION_NONE);
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

		void setDialogueAction(DialogueAction action);
		DialogueAction getDialogueAction() const;

		static bool addDialogue(Dialogue *dialogue);
		static bool removeDialogue(const char *id);
		static void removeAllDialogue();
		static Dialogue *getDialogue(const char *id);

		static void getAvailableDialogues(vector<Dialogue *> &result, const GameObject *talker, const GameObject *talkedTo);

		static UnlockFlag toUnlockFlag(const char *flag);
		static UnlockFlag toUnlockFlag(int flagValue);
		static const char *getUnlockFlagName(UnlockFlag flag);

		static DialogueAction toDialogueAction(const char *action);
		static DialogueAction toDialogueAction(int actionValue);
		static const char *getDialogueActionName(DialogueAction action);

	protected:

		string mText;
		string mId;
		string mTitle;
		string mSubject;
		DialogueAction mAction;
		UnlockFlag mUnlockFlag;

		typedef map<string, Dialogue *> DialogueMap;
		static DialogueMap sDialogueMap;

	};

}
}
