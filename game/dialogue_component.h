#pragma once

#include <base/handle.h>
#include <base/imanaged.h>
using am::base::Handle;
using am::base::IManaged;

#include <map>
#include <string>
using std::map;
using std::string;

#include <ui/ievent_manager.h>
using am::ui::IEventManager;

#include <game/dialogue.h>

namespace am {
namespace game {

	class GameObject;

	class DialogueComponent : public IManaged {
	public:

		typedef map<string, bool> SubjectMap;

		DialogueComponent(GameObject *attachedTo = NULL);
		DialogueComponent(const DialogueComponent &copy);
		~DialogueComponent();

		virtual void talkTo(GameObject *other);
		virtual void talkTo(GameObject*other, Dialogue *diag);
		virtual GameObject *getTalkingTo() const;

		virtual void setStartDialogue(Dialogue *diag);
		virtual Dialogue *getStartDialogue() const;

		virtual void setSubjectLock(const char *subject, bool locked = false);
		virtual bool isSubjectLocked(const char *subject) const;
		virtual const SubjectMap &getUnlockedSubjects() const;

		virtual void setDialogueAvailable(const char *subject, bool available = true);
		virtual bool isDialogueAvailable(const char *subject) const;
		virtual const SubjectMap &getDialoguesAvailable() const;

		virtual void setAttachedTo(GameObject *obj);
		virtual GameObject *getAttachedTo() const;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		SubjectMap mUnlockedSubjects;
		SubjectMap mDialoguesAvailable;
		Dialogue *mStartDialogue;
		GameObject *mTalkingTo;
		GameObject *mAttachedTo;

		virtual void setTalkingTo(GameObject *other);

	};

}
}
