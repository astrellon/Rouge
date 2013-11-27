#pragma once

#include <base/handle.h>

#include <gfx/gfx_text_field2.h>
using namespace am::gfx;

#include <vector>
#include <map>
#include <string>

#include <ui/ui_panel.h>

namespace am {
namespace ui {

	class DebugInspector : public Panel {
	public:
		DebugInspector();
		~DebugInspector();

		virtual TextField2 *getTextField();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void onEvent(MouseEvent *e);

		virtual void setValue(std::string key, std::string value);
		virtual std::string getValue(std::string key) const;

		virtual void render(float dt);

		static void setInspector(DebugInspector *inspector);
		static DebugInspector *getInspector();

	protected:

		std::map<std::string, std::string> mValues;
		am::base::Handle<TextField2> mInfo;

		bool mTextDirty;
		void updateText();

		static am::base::Handle<DebugInspector> sInspector;
	};

}
}
