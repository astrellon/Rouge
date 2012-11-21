#include "dialogue_component.h"

#include <ui/dialogue_event.h>
using am::ui::DialogueEvent;

namespace am {
namespace game {

	DialogueComponent::DialogueComponent() :
		mStartDialogue(NULL)
	{
		
	}
	DialogueComponent::~DialogueComponent()
	{

	}

	void DialogueComponent::talkTo(GameObjectComponent *other)
	{
		DialogueComponent *otherComp = other->getComponent<DialogueComponent>();
		if (!otherComp)
		{
			return;
		}
		mTalkingTo = other;
		Handle<DialogueEvent> e(new DialogueEvent(otherComp->getStartDialogue()));
		fireEvent<DialogueEvent>(e);
	}
	void DialogueComponent::talkTo(GameObjectComponent *other, Dialogue *diag)
	{
		mTalkingTo = other;
		Handle<DialogueEvent> e(new DialogueEvent(diag));
		fireEvent<DialogueEvent>(e);
	}
	GameObjectComponent *DialogueComponent::getTalkingTo() const
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

	const char *DialogueComponent::getCompName() const
	{
		return getName();
	}
	Component::ComponentKey DialogueComponent::getCompKey() const
	{
		return getCompKey();
	}

	Component::ComponentKey DialogueComponent::getKey()
	{
		return 1;
	}
	const char *DialogueComponent::getName()
	{
		return "DialogueComponent";
	}
	
}
}
