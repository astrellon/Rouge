#pragma once

#include <base/handle.h>

#include <string>

#include <game/dialogue.h>

#include <gfx/gfx_text_field2.h>

#include "ui_component.h"
#include "ievent_listener.h"

namespace am {
namespace ui {

	class DialogueBox : public UIComponent, public IEventListener
	{
	public:
		DialogueBox();
		~DialogueBox();

		gfx::TextField2 *getTextField() const;

		virtual void onEvent(MouseEvent *e);
		virtual void onEvent(DialogueEvent *e);

		virtual void setTalker(game::GameObject *talker);
		virtual game::GameObject *getTalker() const;

		virtual void setTalkedTo(game::GameObject *talkedTo);
		virtual game::GameObject *getTalkedTo() const;

		virtual void setDialogue(game::Dialogue *dialogue);
		virtual game::Dialogue *getDialogue() const;

		virtual float getMeasuredWidth();
		virtual float getMeasuredHeight();

		virtual float getWidth();
		virtual float getHeight();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void preRender(float dt);

	protected:

		base::Handle<gfx::TextField2> mText;
		game::Dialogue *mDialogue;
		base::Handle<game::GameObject> mTalker;
		base::Handle<game::GameObject> mTalkedTo;

		void updateText();
	};

}
}
