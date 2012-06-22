#pragma once

#include "handle.h"

#include <string>
using namespace std;

#include "gfx/gfx_text_field.h"
using namespace am::gfx;

#include "ui_component.h"

namespace am {
namespace ui {

	class TextInput : public UIComponent {
	public:
		TextInput();
		~TextInput();

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

		virtual float getWidth();
		virtual float getHeight();

	protected:

		am::util::Handle<TextField> mText;
		int mInputPosition;
	};

}
}
