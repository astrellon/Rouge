#include "dialogue.h"

#include "character.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int Dialogue::LUA_ID = LUA_ID_DIALOGUE;
	const char *Dialogue::LUA_TABLENAME = LUA_TABLE_DIALOGUE;

	Dialogue::Dialogue(const char *id, const char *text, const char *title, 
		const char *subject, UnlockFlag unlock, DialogueAction action, const char *actionData) :
		mId(id),
		mText(text),
		mUnlockFlag(unlock),
		mAction(action)
	{
		if (title != nullptr)
		{
			mTitle = title;
		}
		if (subject != nullptr)
		{
			mSubject = subject;
		}
        if (actionData != nullptr)
        {
            mActionData = actionData;
        }
	}
	Dialogue::~Dialogue()
	{
	}

	void Dialogue::setText(const char *text)
	{
		mText = text;
	}
	const char *Dialogue::getText() const
	{
		return mText.c_str();
	}

	void Dialogue::setTitle(const char *title)
	{
		mTitle = title;
	}
	const char *Dialogue::getTitle() const
	{
		if (mTitle.length() > 0)
		{
			return mTitle.c_str();
		}
		return mId.c_str();
	}

	void Dialogue::setId(const char *id)
	{
		mId = id;
	}
	const char *Dialogue::getId() const
	{
		return mId.c_str();
	}

	void Dialogue::setSubject(const char *subject)
	{
		mSubject = subject;
	}
	const char *Dialogue::getSubject() const
	{
		if (mSubject.length() > 0)
		{
			return mSubject.c_str();
		}
		return mId.c_str();
	}

    void Dialogue::setActionData(const char *data)
    {
        mActionData = data;
    }
    const char *Dialogue::getActionData() const
    {
        return mActionData.c_str();
    }

	void Dialogue::setUnlockFlag(Dialogue::UnlockFlag flag)
	{
		mUnlockFlag = flag;
	}
	Dialogue::UnlockFlag Dialogue::getUnlockFlag() const
	{
		return mUnlockFlag;
	}

	void Dialogue::setDialogueAction(DialogueAction action)
	{
		mAction = action;
	}
	Dialogue::DialogueAction Dialogue::getDialogueAction() const
	{
		return mAction;
	}

	Dialogue::UnlockFlag Dialogue::toUnlockFlag(const char *flag)
	{
		if (strcmp("none", flag) == 0)
		{
			return UNLOCK_NONE;
		}
		if (strcmp("locked", flag) == 0)
		{
			return UNLOCK_LOCKED;
		}
		return UNLOCK_UNKNOWN;
	}
	Dialogue::UnlockFlag Dialogue::toUnlockFlag(int flagValue)
	{
		if (flagValue < 0 || flagValue >= UNLOCK_UNKNOWN)
		{
			return UNLOCK_UNKNOWN;
		}
		return static_cast<UnlockFlag>(flagValue);
	}
	const char *Dialogue::getUnlockFlagName(Dialogue::UnlockFlag flag)
	{
		if (flag == UNLOCK_NONE)
		{
			return "none";
		}
		if (flag == UNLOCK_LOCKED)
		{
			return "locked";
		}
		return "unknown";
	}

	Dialogue::DialogueAction Dialogue::toDialogueAction(const char *action)
	{
		if (strcmp("none", action) == 0)
		{
			return ACTION_NONE;
		}
		if (strcmp("shop", action) == 0)
		{
			return ACTION_SHOP;
		}
		if (strcmp("close", action) == 0)
		{
			return ACTION_CLOSE;
		}
		return ACTION_UNKNOWN;
	}
	Dialogue::DialogueAction Dialogue::toDialogueAction(int actionValue)
	{
		if (actionValue < 0 || actionValue >= ACTION_UNKNOWN)
		{
			return ACTION_UNKNOWN;
		}
		return static_cast<DialogueAction>(actionValue);
	}
	const char *Dialogue::getDialogueActionName(Dialogue::DialogueAction action)
	{
		if (action == Dialogue::ACTION_NONE)
		{
			return "none";
		}
		if (action == Dialogue::ACTION_CLOSE)
		{
			return "close";
		}
		if (action == Dialogue::ACTION_SHOP)
		{
			return "shop";
		}
		return "unknown";
	}
	
}
}
