#pragma once

#include <base/imanaged.h>

#include <string>
#include <map>
#include <vector>

#include <ui/event_interface.h>

namespace am {
namespace game {

	class GameObject;

	class Dialogue : public ui::EventInterface
	{
	public:

		enum UnlockFlag 
		{
			UNLOCK_NONE, UNLOCK_LOCKED, UNLOCK_UNKNOWN
		};

		enum DialogueAction 
		{
			ACTION_NONE, ACTION_SHOP, ACTION_CLOSE, ACTION_UNKNOWN
		};

		// Is a list of dialogue subjects
		//typedef std::vector<string> DialogueChoices;

		Dialogue(const char *id, const char *text, const char *title = nullptr, const char *subject = nullptr, UnlockFlag unlock = UNLOCK_NONE, DialogueAction action = ACTION_NONE, const char *actionData = nullptr);

		~Dialogue();

		void setText(const char *text);
		const char *getText() const;

		void setTitle(const char *title);
		const char *getTitle() const;

		void setId(const char *id);
		const char *getId() const;

		void setSubject(const char *subject);
		const char *getSubject() const;

        void setActionData(const char *data);
        const char *getActionData() const;

		void setUnlockFlag(UnlockFlag flag);
		UnlockFlag getUnlockFlag() const;

		void setDialogueAction(DialogueAction action);
		DialogueAction getDialogueAction() const;

		static UnlockFlag toUnlockFlag(const char *flag);
		static UnlockFlag toUnlockFlag(int flagValue);
		static const char *getUnlockFlagName(UnlockFlag flag);

		static DialogueAction toDialogueAction(const char *action);
		static DialogueAction toDialogueAction(int actionValue);
		static const char *getDialogueActionName(DialogueAction action);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		std::string mText;
		std::string mId;
		std::string mTitle;
		std::string mSubject;
        std::string mActionData;
		DialogueAction mAction;
		UnlockFlag mUnlockFlag;

	};

}
}

