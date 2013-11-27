#pragma once

#include <base/handle.h>

#include <string>
using namespace std;

#include <game/dialogue.h>
using namespace am::game;

#include <gfx/gfx_text_field2.h>
using namespace am::gfx;

#include "ui_component.h"
#include "ievent_listener.h"

namespace am {
namespace ui {

	class DialogueChoices : public UIComponent, public IEventListener {
	public:
		DialogueChoices();
		~DialogueChoices();

		TextField2 *getTextField() const;

		virtual void onEvent(MouseEvent *e);
		virtual void onEvent(DialogueEvent *e);

		virtual void setTalker(GameObject *talker);
		virtual GameObject *getTalker() const;

		virtual void setTalkedTo(GameObject *talkedTo);
		virtual GameObject *getTalkedTo() const;

		virtual void setDialogueChoices(const vector<Dialogue *> &choices);
		virtual const vector<Dialogue *> &getDialogueChoices() const;

		virtual float getMeasuredWidth();
		virtual float getMeasuredHeight();

		virtual float getWidth();
		virtual float getHeight();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void preRender(float dt);
		virtual void postRender(float dt);

	protected:

		am::base::Handle<TextField2> mText;
		vector<Dialogue *> mChoices;
		am::base::Handle<GameObject> mTalker;
		am::base::Handle<GameObject> mTalkedTo;

		void updateText();
	};

}
}
