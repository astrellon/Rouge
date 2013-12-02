#pragma once

#include <base/handle.h>

#include <gfx/gfx_text_field2.h>
#include <gfx/gfx_sprite.h>
#include <gfx/gfx_tooltip.h>

#include <string>

namespace am {
namespace gfx {

	class TooltipString : public Tooltip
	{
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

		base::Handle<TextField2> mTextField;
		base::Handle<TextField2> mDetailedTextField;
	};

}
}
