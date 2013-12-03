#pragma once

#include <base/handle.h>

#include <string>

#include <game/dialogue.h>

#include <gfx/gfx_text_field2.h>

#include "ui_component.h"
#include "ievent_listener.h"

namespace am {
namespace ui {

	class DialogueChoices : public UIComponent, public IEventListener 
	{
	public:
		DialogueChoices();
		~DialogueChoices();

		gfx::TextField2 *getTextField() const;

		virtual void onEvent(MouseEvent *e);
		virtual void onEvent(DialogueEvent *e);

		virtual void setTalker(game::GameObject *talker);
		virtual game::GameObject *getTalker() const;

		virtual void setTalkedTo(game::GameObject *talkedTo);
		virtual game::GameObject *getTalkedTo() const;

		virtual void setDialogueChoices(const std::vector<game::Dialogue *> &choices);
		virtual const std::vector<game::Dialogue *> &getDialogueChoices() const;

		virtual float getMeasuredWidth();
		virtual float getMeasuredHeight();

		virtual float getWidth();
		virtual float getHeight();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void preRender(float dt);
		virtual void postRender(float dt);

	protected:

		base::Handle<gfx::TextField2> mText;
		std::vector<game::Dialogue *> mChoices;
		base::Handle<game::GameObject> mTalker;
		base::Handle<game::GameObject> mTalkedTo;

		void updateText();
	};

}
}
