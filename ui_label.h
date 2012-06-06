#pragma once

#include "ui_component.h"

#include <string>

using namespace std;

namespace am {
namespace gfx {
	class GfxEngine;
	class TextField;
}

using namespace am::gfx;

namespace ui {

	class Label : public UIComponent {
	public:
		Label(GfxEngine *engine);
		Label(GfxEngine *engine, const char *label);
		Label(GfxEngine *engine, const string &label);
		~Label();

		void setLabel(const char *label);
		void setLabel(const string &label);
		string getLabel() const;

		TextField *getLabelField();

		virtual float getWidth();
		virtual float getHeight();

	protected:

		TextField *mLabel;
	};

}
}
