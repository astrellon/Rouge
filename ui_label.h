#pragma once

#include "ui_component.h"

#include <string>
#include "handle.h"

using namespace std;

namespace am {
namespace gfx {
	class TextField;
}

using namespace am::gfx;

namespace ui {

	class Label : public UIComponent {
	public:
		Label();
		Label(const char *label);
		Label(const string &label);
		~Label();

		void setLabel(const char *label);
		void setLabel(const string &label);
		string getLabel() const;

		TextField *getLabelField();

		virtual float getWidth();
		virtual float getHeight();

	protected:

		am::util::Handle<TextField> mLabel;
	};

}
}
