#include "dialogue_component.h"

#include <ui/dialogue_event.h>
using am::ui::DialogueEvent;

#include <game/game_object.h>

namespace am {
namespace game {

	DialogueComponent::DialogueComponent(GameObject *attached) :
		mStartDialogue(NULL),
		mTalkingTo(NULL),
		mAttachedTo(NULL)
	{
		setAttachedTo(attached);
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
		Handle<DialogueEvent> e(new DialogueEvent(otherComp->getStartDialogue()));
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
		Handle<DialogueEvent> e(new DialogueEvent(diag));
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
			return false;
		}
		string subjectStr(subject);
		SubjectMap::const_iterator iter = mUnlockedSubjects.find(subjectStr);
		if (iter == mUnlockedSubjects.end())
		{
			return false;
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

}
}
