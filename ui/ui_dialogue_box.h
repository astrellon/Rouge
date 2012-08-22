#pragma once

#include <base/handle.h>
using namespace am::base;

#include <string>
using namespace std;

#include <gfx/gfx_text_field2.h>
using namespace am::gfx;

#include "ui_component.h"
#include "ievent_listener.h"

namespace am {
namespace game {
	class Dialogue;
}
using namespace am::game;

namespace ui {

	class DialogueBox : public UIComponent, public IEventListener {
	public:
		DialogueBox();
		~DialogueBox();

		TextField2 *getTextField() const;

		virtual void onEvent(MouseEvent *e);

		virtual void setDialogue(Dialogue *dialogue);
		virtual Dialogue *getDialogue() const;

		virtual float getMeasuredWidth();
		virtual float getMeasuredHeight();

		virtual float getWidth();
		virtual float getHeight();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

	protected:

		Handle<TextField2> mText;
		Dialogue *mDialogue;

		void updateText();
	};

}
}
