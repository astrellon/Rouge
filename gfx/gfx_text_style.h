#pragma once

#include <base/handle.h>

#include <util/colour.h>

#include <gfx/gfx_font.h>
#include <gfx/gfx_text_style_selector.h>

#include <lua/lua_state.h>

namespace am {
namespace gfx {

	class TextStyle {
	public:

		TextStyle();
		TextStyle(const util::Colour &colour);
		~TextStyle();

		void setColour(const util::Colour &colour);
		util::Colour getColour() const;
		void removeColour();
		bool hasColour() const;

		bool loadDef(lua::LuaState &lua);
		
		static TextStyle getCalcStyle(const TextStyleSelector &node);
		static TextStyle *getStyle(const TextStyleSelector &node);
		static void addStyle(const TextStyleSelector &node, const TextStyle &style);
		static bool removeStyle(const TextStyleSelector &node);

		static void loadStylesLua(const char *filename);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		util::Colour mColour;
		
		typedef std::pair<TextStyleSelector, TextStyle> NodeStylePair;
		typedef std::vector< NodeStylePair > NodeStyleList;
		static NodeStyleList sNodeStyleList;

		static int parseStyleTable(lua::LuaState &lua);

	};

}
}
