#include "dialogue_component.h"

#include <sstream>

#include <ui/dialogue_event.h>
using am::ui::DialogueEvent;

#include <util/data_map.h>
#include <util/data_string.h>
#include <util/data_array.h>

#include <log/logger.h>

#include "game_object.h"
#include "loading_state.h"

namespace am {
namespace game {

	const int DialogueComponent::LUA_ID = 0x15;
	const char *DialogueComponent::LUA_TABLENAME = "am_game_DialogueComponent";

	DialogueComponent::DialogueComponent(GameObject *attached) :
		mStartDialogue(NULL),
		mTalkingTo(NULL),
		mAttachedTo(NULL)
	{
		setAttachedTo(attached);
	}
	DialogueComponent::DialogueComponent(const DialogueComponent &copy) :
		mStartDialogue(copy.mStartDialogue),
		mTalkingTo(NULL),
		mAttachedTo(NULL),
		mUnlockedSubjects(copy.mUnlockedSubjects),
		mDialoguesAvailable(copy.mDialoguesAvailable)
	{

	}
	DialogueComponent::~DialogueComponent()
	{

	}

	void DialogueComponent::talkTo(GameObject *other)
	{
		if (mAttachedTo == NULL || other == NULL)
		{
			return;
		}
		DialogueComponent *otherComp = other->getDialogueComp();
		if (!otherComp)
		{
			return;
		}
		setTalkingTo(other);
		Handle<DialogueEvent> e(new DialogueEvent(otherComp->getStartDialogue(), false));
		mAttachedTo->fireEvent<DialogueEvent>(e);
		other->fireEvent<DialogueEvent>(e);

		e = new DialogueEvent(otherComp->getStartDialogue(), true);
		mAttachedTo->fireEvent<DialogueEvent>(e);
		other->fireEvent<DialogueEvent>(e);
	}
	void DialogueComponent::talkTo(GameObject *other, Dialogue *diag)
	{
		if (mAttachedTo == NULL || other == NULL)
		{
			return;
		}
		setTalkingTo(other);
		Handle<DialogueEvent> e(new DialogueEvent(diag, false));
		mAttachedTo->fireEvent<DialogueEvent>(e);
		other->fireEvent<DialogueEvent>(e);

		e = new DialogueEvent(diag, true);
		mAttachedTo->fireEvent<DialogueEvent>(e);
		other->fireEvent<DialogueEvent>(e);
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
		if (subject == NULL || subject[0] == '\0')
		{
			return;
		}
		string subjectStr(subject);
		mUnlockedSubjects[subjectStr] = locked;
	}
	bool DialogueComponent::isSubjectLocked(const char *subject) const
	{
		if (subject == NULL || subject[0] == '\0')
		{
			return true;
		}
		string subjectStr(subject);
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
		if (subject == NULL || subject[0] == '\0')
		{
			return;
		}
		string subjectStr(subject);
		mDialoguesAvailable[subjectStr] = available;
	}
	bool DialogueComponent::isDialogueAvailable(const char *subject) const
	{
		if (subject == NULL || subject[0] == '\0')
		{
			return false;
		}
		string subjectStr(subject);
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
		if (mAttachedTo != NULL)
		{
			mAttachedTo->release();
		}
		mAttachedTo = attached;
		if (mAttachedTo != NULL)
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
		if (mTalkingTo != NULL)
		{
			mTalkingTo->release();
		}
		mTalkingTo = other;
		if (mTalkingTo != NULL)
		{
			mTalkingTo->retain();
		}
	}

	data::IData *DialogueComponent::serialise()
	{
		data::Map *output = new data::Map();
		data::Array *unlocked = new data::Array();
		for (auto iter = mUnlockedSubjects.begin(); iter != mUnlockedSubjects.end(); ++iter)
		{
			// If not locked
			if (!iter->second)
			{
				unlocked->push(iter->first);
			}
		}
		output->push("unlockedSubjects", unlocked);

		data::Array *available = new data::Array();
		for (auto iter = mDialoguesAvailable.begin(); iter != mDialoguesAvailable.end(); ++iter)
		{
			available->push(iter->first);
		}
		output->push("dialoguesAvailable", available);

		if (mStartDialogue)
		{
			output->push("startDialogue", mStartDialogue->getId());
		}

		return output;
	}

	void DialogueComponent::deserialise(LoadingState *state, data::IData *data)
	{
		if (!data)
		{
			return;
		}
		Handle<data::Map> dataMap(dynamic_cast<data::Map *>(data));
		if (!dataMap)
		{
			stringstream ss;
			ss << "Unable to load dialogue component from '" << data->typeName();
			ss << "' expected a Map.";
			am_log("LOADERR", ss);
			return;
		}
		Handle<data::Array> unlocked(dataMap->at<data::Array>("unlockedSubjects"));
		if (unlocked)
		{
			for (auto iter = unlocked->begin(); iter != unlocked->end(); ++iter)
			{
				if ((*iter)->type() == data::String::TYPE)
				{
					setSubjectLock((*iter)->string());
				}
				else
				{
					stringstream ss;
					ss << "Unlocked subject array element must be a String not a '";
					ss << (*iter)->typeName() << '\'';
					am_log("LOADERR", ss);
				}
			}
		}

		Handle<data::Array> available(dataMap->at<data::Array>("dialoguesAvailable"));
		if (available)
		{
			for (auto iter = available->begin(); iter != available->end(); ++iter)
			{
				if ((*iter)->type() == data::String::TYPE)
				{
					setDialogueAvailable((*iter)->string());
				}
				else
				{
					stringstream ss;
					ss << "Dialogue available array element must be a String not a '";
					ss << (*iter)->typeName() << '\'';
					am_log("LOADERR", ss);
				}
			}
		}

		Handle<data::String> start(dataMap->at<data::String>("startDialogue"));
		if (start)
		{
			state->setStartDialogue(start->string(), this);
		}
	}

}
}
