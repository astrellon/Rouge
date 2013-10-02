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
	 * Simple wrapper for the sprite class, currently only allows for changing
	 * the asset used by the sprite by name.
	 */
	/**
	 * @static
	 * Creates a new sprite with no asset.
	 */
	/**
	 * @static
	 * Creates a new sprite using the given asset name.
	 *
	 * @param string asset_name Name of the asset for this sprite to use.
	 */
	int Sprite_ctor(lua_State *lua)
	{
		if (lua_gettop(lua) == 1)
		{
			if (lua_isstr(lua, 1))
			{
				Sprite *sprite = new Sprite(lua_tostring(lua, 1));
				wrapRefObject<Sprite>(lua, sprite);
				return 1;
			}
			return LuaState::expectedArgs(lua, "@new", "string asset_name");
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
	 * @param am.sprite rhs The other sprite to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int Sprite_eq(lua_State *lua)
	{
		Sprite *lhs = castUData<Sprite>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.sprite");
		}
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
			{ "frame_rate", nullptr },
			{ "current_frame", nullptr },
			{ "frame_time", nullptr },
			{ "width", nullptr },
			{ "height", nullptr },
			// Renderable methods
			{ "size", nullptr },
			{ "parent", nullptr },
			{ "interactive", nullptr },
			{ "visible", nullptr },
			{ "position", nullptr },
			{ "screen_to_local", nullptr },
			{ "local_to_screen", nullptr },
			{ nullptr, nullptr }
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
	 * @returns string The asset name is use.
	 */
	/**
	 * Sets the asset name for this sprite to use.
	 *
	 * @param string asset_name The name of the asset to use.
	 * @returns am.sprite This
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
			else if (lua_isstr(lua, 2))
			{
				sprite->setAsset(lua_tostring(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "asset", "string asset_name");
		}
		return LuaState::expectedContext(lua, "asset", "am.sprite");
	}

	bool getSprite(lua_State *lua, int n, Sprite *&result)
	{
		if (lua_isstr(lua, n))
		{
			result = new Sprite(lua_tostring(lua, n));
			return true;
		}
		else if (lua_isnil(lua, n))
		{
			result = nullptr;
			return true;
		}
		Sprite *sprite = castUData<Sprite>(lua, n);
		result = sprite;
		return sprite != nullptr;
	}
}
}
}
