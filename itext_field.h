#pragma once

#include "igfx_component.h"
#include "irenderable.h"

#include <string>

using namespace std;

namespace am {
namespace gfx {

	class IGfxEngine;
	class IFont;

	class ITextField : public IRenderable {
		virtual IFont *getBaseFont() = 0;
		virtual void setBaseFont(IFont *font) = 0;

		virtual void setText(string &str) = 0;
		virtual void appendText(string &str) = 0;
		virtual string getText() = 0;

	};

}
}
