#include "lua_font.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <gfx/gfx_font.h>
#include <gfx/gfx_asset.h>
#include <gfx/gfx_engine.h>
using namespace am::gfx;

namespace am {
namespace lua {
namespace gfx {
	/**
	 * @class
	 * Simple wrapper for the font class. Before a font can be used it must have process() called.
	 */
	/**
	 * @static
	 * Creates a new font using the given name.
	 *
	 * @param string font_name Name of the font.
	 */
	int Font_ctor(lua_State *lua)
	{
		if (lua_isstr(lua, 1))
		{
			Font *font = new Font(lua_tostring(lua, 1));
			wrapRefObject<Font>(lua, font);
			return 1;
		}
		return LuaState::expectedArgs(lua, "@new", "string font_name");
	}
	/**
	 * Releases the reference counter on the font.
	 */
	int Font_dtor(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			font->release();
		}
		return 0;
	}
	/**
	 * Compares this font with another font object.
	 *
	 * @param am.font rhs The other font to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int Font_eq(lua_State *lua)
	{
		Font *lhs = castUData<Font>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.font");
		}
		Font *rhs = castUData<Font>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Font_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Font_ctor },
			{ "__gc", Font_dtor },
			{ "__eq", Font_eq },
			{ "asset", Font_asset },
			{ "name", Font_name },
			{ "fixed_width", Font_fixed_width },
			{ "char_width", Font_char_width },
			{ "char_height", Font_char_height },
			{ "kerning", Font_kerning },
			{ "leading", Font_leading },
			{ "space_width", Font_space_width },
			{ "tab_width", Font_tab_width },
			{ "measure_text", Font_measure_text },
			{ "measure_line", Font_measure_line },
			{ "measure_word", Font_measure_word },
			{ "process", Font_process },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, Font::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	/**
	 * Returns the asset name currently used by this font.
	 * An empty string indicates that no asset has been set.
	 *
	 * @returns string The asset name is use.
	 */
	/**
	 * Sets the asset name for this sprite to use.
	 *
	 * @param string asset_name The name of the asset to use.
	 * @returns am.font This
	 */
	int Font_asset(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			if (lua_gettop(lua) == 1)
			{
				base::Handle<Asset> asset(font->getAsset());
				if (asset)
				{
					lua_pushstring(lua, asset->getName());
					return 1;
				}
				lua_pushstring(lua, "");
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				font->setAsset(GfxEngine::getEngine()->getAsset(lua_tostring(lua, 2)));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "asset", "string asset_name");
		}
		return LuaState::expectedContext(lua, "asset", "am.font");
	}
	
	/**
	 * Returns the name of the font.
	 *
	 * @returns string The name of the font.
	 */
	int Font_name(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			lua_pushstring(lua, font->getName().c_str());
			return 1;
		}
		return LuaState::expectedContext(lua, "name", "am.font");
	}
	/**
	 * Returns if the font is a monospaced/fixed width font.
	 *
	 * @returns boolean True if a fixed width font.
	 */
	/**
	 * Sets if the font should be fixed width or variable width.
	 * The width of each character is calculated based on the amount of transparent
	 * pixels on either side of each character.
	 *
	 * @param boolean fixed_width Weather the font is fixed width or not.
	 * @returns am.font This
	 */
	int Font_fixed_width(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushboolean(lua, font->isFixedWidth());
				return 1;
			}
			else if (lua_isbool(lua, 2))
			{
				font->setFixedWidth(lua_tobool(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "fixed_width", "boolean fixed_width");
		}
		return LuaState::expectedContext(lua, "fixed_width", "am.font");
	}
	/**
	 * Returns the pixel width of the given character. This will always be
	 * the same for mono spaced fonts. The char_code should be an ASCII value.
	 *
	 * @param integer char_code The ASCII character to check.
	 * @returns number The width of the given character.
	 */
	/**
	 * Returns the pixel width of the first character in the given string. This will always be
	 * the same for mono spaced fonts.
	 *
	 * @param string char The character to check.
	 * @returns number The width of the given character.
	 */
	int Font_char_width(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			if (lua_isnum(lua, 2))
			{
				lua_pushnumber(lua, font->getCharWidth(lua_tointeger(lua, 2)));
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				lua_pushnumber(lua, font->getCharWidth(lua_tostring(lua, 2)[0]));
				return 1;
			}
			return LuaState::expectedArgs(lua, "char_width", 2, "integer char_code", "string char");
		}
		return LuaState::expectedContext(lua, "char_width", "am.font");
	}
	/**
	 * Returns the pixel height of each character.
	 * Each character is considered to have the same height.
	 *
	 * @returns number The character height.
	 */
	int Font_char_height(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			lua_pushnumber(lua, font->getCharHeight());
			return 1;
		}
		return LuaState::expectedContext(lua, "char_height", "am.font");
	}
	/**
	 * Returns the kerning of the font.
	 *
	 * @returns number The kerning.
	 */
	/**
	 * Sets the kerning of the font.
	 *
	 * @param number kerning The kerning to set.
	 * @returns am.font This
	 */
	int Font_kerning(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushnumber(lua, font->getKerning());
				return 1;
			}
			else if (lua_isnum(lua, 2))
			{
				font->setKerning(lua_tonumber(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "kerning", "number kerning");
		}
		return LuaState::expectedContext(lua, "kerning", "am.font");
	}

	/**
	 * Returns the leading of the font.
	 *
	 * @returns number The leading.
	 */
	/**
	 * Sets the leading of the font.
	 *
	 * @param number leading The leading to set.
	 * @returns am.font This
	 */
	int Font_leading(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushnumber(lua, font->getLeading());
				return 1;
			}
			else if (lua_isnum(lua, 2))
			{
				font->setLeading(lua_tonumber(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "leading", "number leading");
		}
		return LuaState::expectedContext(lua, "leading", "am.font");
	}

	/**
	 * Returns the space width of the font.
	 *
	 * @returns number The space width.
	 */
	/**
	 * Sets the space width of the font.
	 *
	 * @param number space_width The space width to set.
	 * @returns am.font This
	 */
	int Font_space_width(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushnumber(lua, font->getSpaceWidth());
				return 1;
			}
			else if (lua_isnum(lua, 2))
			{
				font->setSpaceWidth(lua_tonumber(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "space_width", "number space_width");
		}
		return LuaState::expectedContext(lua, "space_width", "am.font");
	}

	/**
	 * Returns the tab width of the font.
	 *
	 * @returns number The tab_width.
	 */
	/**
	 * Sets the tab width of the font.
	 *
	 * @param number tab_width The tab_width to set.
	 * @returns am.font This
	 */
	int Font_tab_width(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushnumber(lua, font->getTabWidth());
				return 1;
			}
			else if (lua_isnum(lua, 2))
			{
				font->setTabWidth(lua_tonumber(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "tab_width", "number tab_width");
		}
		return LuaState::expectedContext(lua, "tab_width", "am.font");
	}

	/**
	 * Measures the space in pixels that the given block of text would take up when contained
	 * to the given width.
	 *
	 * The end_character is the final character to measure up to. Defaults to the null character
	 * which indicates the end of the string. Can be changed to "\n"/13 (new line) or "."/46 (period) or even "A"/65 (A).
	 *
	 * @param string text The text to measure.
	 * @param number contain_width The width of the container.
	 * @param string/integer [\0] end_character The ending character.
	 * @returns number The width that would be taken up.
	 * @returns number The height that would be taken up.
	 */
	int Font_measure_text(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			if (lua_isnum(lua, 2))
			{
				float width = 0.0f, height = 0.0f;
				char endChar = '\0';
				if (lua_isnum(lua, 3))
				{
					endChar = static_cast<char>(lua_tointeger(lua, 3));
				}
				else if (lua_isstr(lua, 3))
				{
					endChar = lua_tostring(lua, 3)[0];
				}
				font->measureText(lua_tostring(lua, 2), lua_tonumber(lua, 3), width, height, endChar);
				lua_pushnumber(lua, width);
				lua_pushnumber(lua, height);
				return 2;
			}
			return LuaState::expectedArgs(lua, "measure_text", "string text, number container_width, string/integer [\0] end_character");
		}
		return LuaState::expectedContext(lua, "measure_text", "am.font");
	}
	/**
	 * Measures the space in pixels that the given line of text would take up when contained
	 * to the given width. Essentially calls measure text with the end character being a new line.
	 *
	 * @param string text The text to measure.
	 * @param number contain_width The width of the container.
	 * @returns number The width that would be taken up.
	 * @returns number The height that would be taken up.
	 */
	int Font_measure_line(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			if (lua_isnum(lua, 2))
			{
				float width = 0.0f, height = 0.0f;
				font->measureLine(lua_tostring(lua, 2), lua_tonumber(lua, 3), width, height);
				lua_pushnumber(lua, width);
				lua_pushnumber(lua, height);
				return 2;
			}
			return LuaState::expectedArgs(lua, "measure_line", "string text, number container_width");
		}
		return LuaState::expectedContext(lua, "measure_line", "am.font");
	}

	/**
	 * Measures the space in pixels that the given word.
	 *
	 * @param string word The word to measure.
	 * @returns number The width that would be taken up.
	 * @returns number The height that would be taken up.
	 */
	int Font_measure_word(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			if (lua_isnum(lua, 2))
			{
				float width = 0.0f, height = 0.0f;
				font->measureWord(lua_tostring(lua, 2), width, height);
				lua_pushnumber(lua, width);
				lua_pushnumber(lua, height);
				return 2;
			}
			return LuaState::expectedArgs(lua, "measure_word", "string text");
		}
		return LuaState::expectedContext(lua, "measure_word", "am.font");
	}

	/**
	 * Processes the font based on it's current values. This must be done after any changes
	 * are made to the font, including when the font is being created. This calculates all
	 * the character sizes and creates all the texture windows for each character.
	 *
	 * @returns am.font This
	 */
	int Font_process(lua_State *lua)
	{
		Font *font = castUData<Font>(lua, 1);
		if (font)
		{
			font->process();
			lua_first(lua);
		}
		return LuaState::expectedContext(lua, "process", "am.font");
	}
}
}
}
