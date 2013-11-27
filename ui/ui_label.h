#pragma once

#include <base/handle.h>

#include <string>
using namespace std;

#include "ui_component.h"

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

		virtual float getMeasuredWidth();
		virtual float getMeasuredHeight();

		virtual float getWidth();
		virtual float getHeight();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

	protected:

		am::base::Handle<TextField> mLabel;
	};

}
}
