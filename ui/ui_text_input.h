#pragma once

#include <base/handle.h>
using namespace am::base;

#include <string>
using namespace std;

#include <gfx/gfx_text_field.h>
using namespace am::gfx;

#include "ui_component.h"
#include "ievent_listener.h"

namespace am {
namespace ui {

	class TextInput : public UIComponent, public IEventListener {
	public:
		TextInput();
		~TextInput();

		virtual void onEvent(KeyboardEvent *e);

		void setText(const char *text);
		void setText(const string &text);
		string getText() const;

		void appendText(char text);
		void appendText(const char *text);
		void appendText(const string &text);

		int getInputPosition() const;
		void moveInputPosition(int move);
		void setInputPosition(int pos);

		TextField *getTextField();

		void setFocus(bool focus);
		bool hasFocus() const;

		virtual float getWidth();
		virtual float getHeight();

	protected:

		bool mFocus;
		Handle<TextField> mText;
		int mInputPosition;

		void updateInputCursor(int diff);
	};

}
}
