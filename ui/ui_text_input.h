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

		enum Restriction {
			NONE, FLOAT, INTEGER
		};
		TextInput();
		TextInput(const TextInput &copy);
		~TextInput();

		virtual Renderable *clone() const;

		virtual void onEvent(MouseEvent *e);
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

		void setRestriction(Restriction restriction);
		Restriction getRestriction() const;

		TextField *getTextField();

		void setFocus(bool focus);
		bool hasFocus() const;

		void setMaxCharacters(int maxCharacters, bool deleteExtra = false);
		int getMaxCharacters() const;

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual float getWidth();
		virtual float getHeight();

	protected:

		bool mFocus;
		Handle<TextField> mText;
		int mInputPosition;
		int mMaxCharacters;
		Restriction mRestriction;

		void updateInputCursor(int diff);

		virtual void preRender(float dt);

		void fireChangeEvent();

		bool validateChar(char input);
		bool validateText(const char *input);
	};

}
}
