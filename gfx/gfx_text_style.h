#pragma once

#include <base/handle.h>
using namespace am::base;

#include <util/colour.h>
using namespace am::util;

#include <gfx/gfx_font.h>
#include <gfx/gfx_text_style_selector.h>

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace gfx {

	class TextStyle {
	public:

		TextStyle();
		TextStyle(const Colour &colour);
		~TextStyle();

		void setColour(const Colour &colour);
		Colour getColour() const;
		void removeColour();
		bool hasColour() const;

		//bool loadDef(JsonValue value);
		bool loadDef(LuaState &lua);
		
		static TextStyle getCalcStyle(const TextStyleSelector &node);
		static TextStyle *getStyle(const TextStyleSelector &node);
		static void addStyle(const TextStyleSelector &node, const TextStyle &style);
		static bool removeStyle(const TextStyleSelector &node);

		//static void loadStyles(const char *filename);
		static void loadStylesLua(const char *filename);

	protected:

		Colour mColour;
		
		typedef pair<TextStyleSelector, TextStyle> NodeStylePair;
		typedef vector< NodeStylePair > NodeStyleList;
		static NodeStyleList sNodeStyleList;

		static int parseStyleTable(LuaState &lua);

	};

}
}
