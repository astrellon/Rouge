#pragma once

#include <base/handle.h>
#include <base/imanaged.h>

#include <map>
#include <string>

#include <ui/ievent_manager.h>
#include <ui/event_interface.h>

#include <game/dialogue.h>

namespace am {
namespace util {
namespace data {
	class IData;
}
}
using namespace am::util;

namespace game {

	class GameObject;
	class LoadingState;

	class DialogueComponent : public ui::EventInterface
	{
	public:

		typedef std::map<std::string, bool> SubjectMap;

		DialogueComponent(GameObject *attachedTo = nullptr);
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

		virtual data::IData *serialise();
		virtual void deserialise(LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		SubjectMap mUnlockedSubjects;
		SubjectMap mDialoguesAvailable;
        base::Handle<Dialogue> mStartDialogue;
		GameObject *mTalkingTo;
		GameObject *mAttachedTo;

		virtual void setTalkingTo(GameObject *other);

	};

}
}
