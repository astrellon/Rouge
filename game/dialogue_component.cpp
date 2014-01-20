#include "dialogue_component.h"

#include <sstream>

#include <ui/dialogue_event.h>

#include <util/data_table.h>
#include <util/data_string.h>

#include <log/logger.h>

#include "game_object.h"
#include "loading_state.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int DialogueComponent::LUA_ID = LUA_ID_DIALOGUECOMP;
	const char *DialogueComponent::LUA_TABLENAME = LUA_TABLE_DIALOGUECOMP;

	DialogueComponent::DialogueComponent(GameObject *attached) :
        ui::EventInterface(),
		mStartDialogue(nullptr),
		mTalkingTo(nullptr),
		mAttachedTo(nullptr)
	{
		setAttachedTo(attached);
	}
	DialogueComponent::DialogueComponent(const DialogueComponent &copy) :
        ui::EventInterface(),
		mStartDialogue(copy.mStartDialogue),
		mTalkingTo(nullptr),
		mAttachedTo(nullptr),
		mUnlockedSubjects(copy.mUnlockedSubjects),
		mDialoguesAvailable(copy.mDialoguesAvailable)
	{

	}
	DialogueComponent::~DialogueComponent()
	{

	}

	void DialogueComponent::talkTo(GameObject *other)
	{
		if (mAttachedTo == nullptr || other == nullptr)
		{
			return;
		}
		DialogueComponent *otherComp = other->getDialogueComp();
		if (!otherComp)
		{
			return;
		}
		setTalkingTo(other);
		base::Handle<ui::DialogueEvent> e(new ui::DialogueEvent(otherComp->getStartDialogue(), false));
		mAttachedTo->fireEvent<ui::DialogueEvent>(e);
		other->fireEvent<ui::DialogueEvent>(e);

		e = new ui::DialogueEvent(otherComp->getStartDialogue(), true);
		mAttachedTo->fireEvent<ui::DialogueEvent>(e);
		other->fireEvent<ui::DialogueEvent>(e);
	}
	void DialogueComponent::talkTo(GameObject *other, Dialogue *diag)
	{
		if (mAttachedTo == nullptr || other == nullptr)
		{
			return;
		}
		setTalkingTo(other);
		base::Handle<ui::DialogueEvent> e(new ui::DialogueEvent(diag, false));
		mAttachedTo->fireEvent<ui::DialogueEvent>(e);
		other->fireEvent<ui::DialogueEvent>(e);

        fireEvent<ui::DialogueEvent>(e);

		e = new ui::DialogueEvent(diag, true);
		mAttachedTo->fireEvent<ui::DialogueEvent>(e);
		other->fireEvent<ui::DialogueEvent>(e);
	}
	GameObject *DialogueComponent::getTalkingTo() const
	{
		return mTalkingTo;
	}

	void DialogueComponent::setStartDialogue(Dialogue *diag)
	{
		mStartDialogue = diag;
	}
	Dialogue *DialogueComponent::getStartDialogue() const
	{
		return mStartDialogue;
	}

	void DialogueComponent::setSubjectLock(const char *subject, bool locked)
	{
		if (subject == nullptr || subject[0] == '\0')
		{
			return;
		}
		std::string subjectStr(subject);
		mUnlockedSubjects[subjectStr] = locked;
	}
	bool DialogueComponent::isSubjectLocked(const char *subject) const
	{
		if (subject == nullptr || subject[0] == '\0')
		{
			return true;
		}
		std::string subjectStr(subject);
		SubjectMap::const_iterator iter = mUnlockedSubjects.find(subjectStr);
		if (iter == mUnlockedSubjects.end())
		{
			return true;
		}
		return iter->second;
	}
	const DialogueComponent::SubjectMap &DialogueComponent::getUnlockedSubjects() const
	{
		return mUnlockedSubjects;
	}

	void DialogueComponent::setDialogueAvailable(const char *subject, bool available)
	{
		if (subject == nullptr || subject[0] == '\0')
		{
			return;
		}
		std::string subjectStr(subject);
		mDialoguesAvailable[subjectStr] = available;
	}
	bool DialogueComponent::isDialogueAvailable(const char *subject) const
	{
		if (subject == nullptr || subject[0] == '\0')
		{
			return false;
		}
		std::string subjectStr(subject);
		SubjectMap::const_iterator iter = mDialoguesAvailable.find(subjectStr);
		if (iter == mDialoguesAvailable.end())
		{
			return false;
		}
		return iter->second;
	}
	const DialogueComponent::SubjectMap &DialogueComponent::getDialoguesAvailable() const
	{
		return mDialoguesAvailable;
	}

	void DialogueComponent::setAttachedTo(GameObject *attached)
	{
		if (mAttachedTo != nullptr)
		{
			mAttachedTo->release();
		}
		mAttachedTo = attached;
		if (mAttachedTo != nullptr)
		{
			mAttachedTo->retain();
		}
	}
	GameObject *DialogueComponent::getAttachedTo() const
	{
		return mAttachedTo;
	}

	void DialogueComponent::setTalkingTo(GameObject *other)
	{
		if (mTalkingTo != nullptr)
		{
			mTalkingTo->release();
		}
		mTalkingTo = other;
		if (mTalkingTo != nullptr)
		{
			mTalkingTo->retain();
		}
	}

	data::IData *DialogueComponent::serialise()
	{
		data::Table *output = new data::Table();
		data::Table *unlocked = new data::Table();
		for (auto iter = mUnlockedSubjects.begin(); iter != mUnlockedSubjects.end(); ++iter)
		{
			// If not locked
			if (!iter->second)
			{
				unlocked->push(iter->first);
			}
		}
		output->at("unlockedSubjects", unlocked);

		data::Table *available = new data::Table();
		for (auto iter = mDialoguesAvailable.begin(); iter != mDialoguesAvailable.end(); ++iter)
		{
			if (iter->second)
			{
				available->push(iter->first);
			}
		}
		output->at("dialoguesAvailable", available);

		if (mStartDialogue)
		{
			output->at("startDialogue", mStartDialogue->getId());
		}

		return output;
	}

	void DialogueComponent::deserialise(LoadingState *state, data::IData *data)
	{
		if (!data)
		{
			return;
		}
		base::Handle<data::Table> dataMap(dynamic_cast<data::Table *>(data));
		if (!dataMap)
		{
			std::stringstream ss;
			ss << "Unable to load dialogue component from '" << data->typeName();
			ss << "' expected a Map.";
			am_log("LOADERR", ss);
			return;
		}
		base::Handle<data::Table> unlocked(dataMap->at<data::Table>("unlockedSubjects"));
		if (unlocked)
		{
			for (auto iter = unlocked->beginArray(); iter != unlocked->endArray(); ++iter)
			{
				if ((*iter)->type() == data::String::TYPE)
				{
					setSubjectLock((*iter)->string());
				}
				else
				{
					std::stringstream ss;
					ss << "Unlocked subject array element must be a String not a '";
					ss << (*iter)->typeName() << '\'';
					am_log("LOADERR", ss);
				}
			}
		}

		base::Handle<data::Table> available(dataMap->at<data::Table>("dialoguesAvailable"));
		if (available)
		{
			for (auto iter = available->beginArray(); iter != available->endArray(); ++iter)
			{
				if ((*iter)->type() == data::String::TYPE)
				{
					setDialogueAvailable((*iter)->string());
				}
				else
				{
					std::stringstream ss;
					ss << "Dialogue available array element must be a String not a '";
					ss << (*iter)->typeName() << '\'';
					am_log("LOADERR", ss);
				}
			}
		}

		base::Handle<data::String> start(dataMap->at<data::String>("startDialogue"));
		if (start)
		{
			state->setStartDialogue(start->string(), this);
		}
	}

}
}
