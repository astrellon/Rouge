#pragma once

#include "ui_component.h"
#include <string>

using namespace std;

namespace am {
namespace gfx {
	class Sprite;
	class TextField;
}

using namespace am::gfx;

namespace ui {

	class Tooltip : public UIComponent {
	public:
		Tooltip();
		Tooltip(const char *tooltip);
		Tooltip(const string &tooltip);
		~Tooltip();

		void setLabel(const char *label);
		void setLabel(const string &label);
		string getLabel() const;

		Sprite *getGraphic();
		TextField *getLabelField();

	protected:
		am::util::Handle<Sprite> mGraphic;
		am::util::Handle<TextField> mLabel;
	};

}
}
