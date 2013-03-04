#include "lua_sprite.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <gfx/gfx_sprite.h>
#include <gfx/gfx_asset.h>
using namespace am::gfx;

namespace am {
namespace lua {
namespace gfx {
	/**
	 * @class
	 * Simple wrapper for the Sprite class, currently only allows for changing
	 * the asset used by the sprite by name.
	 */
	/**
	 * Creates a new sprite with no asset.
	 */
	/**
	 * Creates a new sprite using the given asset name.
	 *
	 * @param String assetName Name of the asset for this sprite to use.
	 */
	int Sprite_ctor(lua_State *lua)
	{
		if (lua_type(lua, 1) == LUA_TSTRING)
		{
			Sprite *sprite = new Sprite(lua_tostring(lua, 1));
			wrapRefObject<Sprite>(lua, sprite);
			return 1;
		}
		Sprite *sprite = new Sprite();
		wrapRefObject<Sprite>(lua, sprite);
		return 1;
	}
	/**
	 * Releases the reference counter on the sprite.
	 */
	int Sprite_dtor(lua_State *lua)
	{
		Sprite *sprite = castUData<Sprite>(lua, 1);
		if (sprite)
		{
			sprite->release();
		}
		return 0;
	}
	/**
	 * Compares this sprite with another sprite object.
	 *
	 * @param Sprite rhs The other sprite to compare with.
	 * @returns Boolean True if they are the same object.
	 */
	int Sprite_eq(lua_State *lua)
	{
		Sprite *lhs = castUData<Sprite>(lua, 1);
		Sprite *rhs = castUData<Sprite>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Sprite_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Sprite_ctor },
			{ "__gc", Sprite_dtor },
			{ "__eq", Sprite_eq },
			{ "asset", Sprite_asset },
			{ "set_frame_rate", NULL },
			{ "get_frame_rate", NULL },
			{ "set_current_frame", NULL },
			{ "get_current_frame", NULL },
			{ "set_frame_time", NULL },
			{ "get_frame_time", NULL },
			{ "get_width", NULL },
			{ "get_height", NULL },
			// Renderable methods
			{ "set_size", NULL },
			{ "get_size", NULL },
			{ "set_width", NULL },
			{ "set_height", NULL },
			{ "set_parent", NULL },
			{ "get_parent", NULL },
			{ "set_interactive", NULL },
			{ "is_interactive", NULL },
			{ "set_visible", NULL },
			{ "is_visible", NULL },
			{ "set_position", NULL },
			{ "get_position", NULL },
			{ "get_screen_to_local", NULL },
			{ "get_local_to_screen", NULL },
			{ NULL, NULL }
		};

		luaL_newmetatable(lua, Sprite::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	/**
	 * Returns the asset name currently used by this sprite.
	 * An empty string indicates that no asset has been set.
	 *
	 * @returns String The asset name is use.
	 */
	/**
	 * Sets the asset name for this sprite to use.
	 *
	 * @param String assetName The name of the asset to use.
	 * @returns Sprite This
	 */
	int Sprite_asset(lua_State *lua)
	{
		Sprite *sprite = castUData<Sprite>(lua, 1);
		if (sprite)
		{
			if (lua_gettop(lua) == 1)
			{
				Handle<Asset> asset(sprite->getAsset());
				if (asset)
				{
					lua_pushstring(lua, asset->getName());
					return 1;
				}
				lua_pushstring(lua, "");
				return 1;
			}
			else if (lua_type(lua, -1) == LUA_TSTRING)
			{
				sprite->setAsset(lua_tostring(lua, -1));
				lua_pushvalue(lua, 1);
				return 1;
			}
		}
		lua_pushnil(lua);
		return 1;
	}

}
}
}
