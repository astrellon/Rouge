#pragma once

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_text_field2.h>
#include <gfx/gfx_sprite.h>
#include <gfx/gfx_tooltip.h>
using namespace am::gfx;

#include <string>
using namespace std;

namespace am {
namespace gfx {

	class TooltipString : public Tooltip {
	public:
		TooltipString();
		TooltipString(const char *tooltip, const char *detailed = nullptr, Renderable *target = nullptr);
		TooltipString(const TooltipString &copy);
		~TooltipString();

		virtual Renderable *clone() const;

		virtual void setText(const char *text);
		virtual const char *getText() const;

		virtual void setDetailedText(const char *text);
		virtual const char *getDetailedText() const;

		virtual TextField2 *getTextField();
		virtual TextField2 *getDetailedTextField();

	protected:

		Handle<TextField2> mTextField;
		Handle<TextField2> mDetailedTextField;
	};

}
}
