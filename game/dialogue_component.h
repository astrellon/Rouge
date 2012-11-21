#pragma once

#include <base/handle.h>
using am::base::Handle;

#include <map>
#include <string>
using std::map;
using std::string;

#include <ui/ievent_manager.h>
using am::ui::IEventManager;

#include <game/component.h>
#include <game/dialogue.h>
#include <game/game_object_component.h>

namespace am {
namespace game {

	class DialogueComponent : public Component, public IEventManager {
	public:

		typedef map<string, bool> SubjectMap;

		DialogueComponent();
		~DialogueComponent();

		virtual void talkTo(GameObjectComponent *other);
		virtual void talkTo(GameObjectComponent *other, Dialogue *diag);
		virtual GameObjectComponent *getTalkingTo() const;

		virtual void setStartDialogue(Dialogue *diag);
		virtual Dialogue *getStartDialogue() const;

		virtual void setSubjectLock(const char *subject, bool locked = false);
		virtual bool isSubjectLocked(const char *subject) const;
		virtual const SubjectMap &getUnlockedSubjects() const;

		virtual void setDialogueAvailable(const char *subject, bool available = true);
		virtual bool isDialogueAvailable(const char *subject) const;
		virtual const SubjectMap &getDialoguesAvailable() const;

		virtual const char *getCompName() const;
		virtual ComponentKey getCompKey() const;

		static ComponentKey getKey();
		static const char *getName();

	protected:

		SubjectMap mUnlockedSubjects;
		SubjectMap mDialoguesAvailable;
		Dialogue *mStartDialogue;
		Handle<GameObjectComponent> mTalkingTo;

	};

}
}
