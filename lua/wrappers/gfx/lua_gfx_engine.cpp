#include "lua_gfx_engine.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>

#include <gfx/gfx_texture.h>
#include <gfx/gfx_asset.h>
#include <gfx/gfx_engine.h>
#include <gfx/gfx_font.h>

#include <math/math.h>

namespace am {
namespace lua {
namespace gfx {
	/**
	 * @class
	 * Simple wrapper for the graphic engine class.
	 * Provides functionality for loading and storing graphical assets.
	 */
	int GfxEngine_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "asset", GfxEngine_asset },
			{ "font", GfxEngine_font },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, am::gfx::GfxEngine::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}

	/**
	 * Returns the asset with the given full name.
	 * The full asset name is made up of the file to load the asset from
	 * and the individual name. For example <code>general:cursor</code><span> If this
	 * asset is not found then the </span><code>data/assets/general.lua</code><span> file
	 * is loaded then the definition is looked for again.</span>
	 *
	 * @param string full_asset_name The full asset name to load.
	 * @returns am.asset The found asset or nil.
	 */
	/**
	 * Registers an asset with the graphics engine.
	 * The name are always namespaced to the file they were loaded from.
	 * For example <code>filename:assetname</code><span> This filename should be relative
	 * to the </span><code>data/assets/</code><span> folder. If no filename is given then the current
	 * file being loaded will be used.
	 * 
	 * The files under </span><code>data/assets/</code><span> will be automatically loaded
	 * when an asset with their filename is used. Assets can also be
	 * registered from outside of those files, however they will need to always
	 * provide a filename. Assets can be overridden.</span>
	 *
	 * @param am.asset asset The asset to store.
	 * @returns am.gfx_engine This
	 */
	int GfxEngine_asset(lua_State *lua)
	{
        am::gfx::GfxEngine *gfxEngine = am::gfx::GfxEngine::getEngine();
		
		if (lua_isstr(lua, 1))
		{
			am::gfx::Asset *asset = gfxEngine->getAsset(lua_tostring(lua, 1));
			if (asset)
			{
				wrapRefObject<am::gfx::Asset>(lua, asset);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		else
		{
			am::gfx::Asset *asset = castUData<am::gfx::Asset>(lua, 1);
			if (asset)
			{
				gfxEngine->addAsset(asset);
				lua_first(lua);
			}
		}
		
		return LuaState::expectedArgs(lua, "asset", 3, "string asset_name", "am.asset asset");
	}

	int GfxEngine_font(lua_State *lua)
	{
		am::gfx::GfxEngine *gfxEngine = am::gfx::GfxEngine::getEngine();
		
		if (lua_isstr(lua, 1))
		{
			am::gfx::Font *font = gfxEngine->getFont(lua_tostring(lua, 1));
			if (font)
			{
				wrapRefObject<am::gfx::Font>(lua, font);
				return 1;
			}
			lua_pushnil(lua);
			return 1;
		}
		else
		{
			am::gfx::Font *font = castUData<am::gfx::Font>(lua, 1);
			if (font)
			{
				gfxEngine->addFont(font);
				lua_first(lua);
			}
		}
		
		return LuaState::expectedArgs(lua, "font", 3, "string font_name", "am.font font");
	}

}
}
}
