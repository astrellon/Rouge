#include "lua_texture.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <gfx/gfx_texture.h>
using namespace am::gfx;

namespace am {
namespace lua {
namespace gfx {
	/**
	 * @class
	 * Simple wrapper for the texture class.
	 */
	/**
	 * @static
	 * Creates a new texture using the given filename.
	 *
	 * @param string filename Path to the texture.
	 */
	int Texture_ctor(lua_State *lua)
	{
		if (lua_gettop(lua) == 1)
		{
			if (lua_isstr(lua, 1))
			{
				Texture *asset = new Texture(lua_tostring(lua, 1));
				wrapRefObject<Texture>(lua, asset);
				return 1;
			}
		}
		return LuaState::expectedArgs(lua, "@new", "string filename");
	}
	/**
	 * Releases the reference counter on the texture.
	 */
	int Texture_dtor(lua_State *lua)
	{
		Texture *asset = castUData<Texture>(lua, 1);
		if (asset)
		{
			asset->release();
		}
		return 0;
	}
	/**
	 * Compares this texture with another texture object.
	 *
	 * @param am.texture rhs The other sprite to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int Texture_eq(lua_State *lua)
	{
		Texture *lhs = castUData<Texture>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.texture");
		}
		Texture *rhs = castUData<Texture>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Texture_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Texture_ctor },
			{ "__gc", Texture_dtor },
			{ "__eq", Texture_eq },
			{ "filename", Texture_filename },
			{ "is_loaded", Texture_is_loaded },
			{ "reload", Texture_reload },
			{ "width", Texture_width },
			{ "height", Texture_height },
			{ "bytes_per_pixel", Texture_bytes_per_pixel },
			{ "gl_format", Texture_gl_format },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, Texture::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	
	/**
	 * Returns the filename of where the texture was loaded.
	 *
	 * @returns string The filename of the texture.
	 */
	/**
	 * Sets the texture from the given filename.
	 * The filename is relative data folder.
	 *
	 * @param string filename The filename to load from.
	 * @returns am.code Return codes:
	 * <table class='return_codes'>
	 *  <tr><td>success</td><td>The texture loaded the new file successfully.</td></tr>
	 *  <tr><td>file_not_found</td><td>Texture didn't load because the file could not be found.</td></tr>
	 *  <tr><td>texture_load_failed</td><td>Texture didn't load because there was an issue with the file.</td></tr>
	 * </table>
	 **/
	int Texture_filename(lua_State *lua)
	{
		Texture *texture = castUData<Texture>(lua, 1);
		if (texture)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, texture->getFilename());
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				lua_pushinteger(lua, texture->setFilename(lua_tostring(lua, 2)));
				return 1;
			}
			return LuaState::expectedArgs(lua, "filename", "string filename");
		}
		return LuaState::expectedContext(lua, "filename", "am.texture");
	}

	/**
	 * Returns true when the texture has loaded successfully.
	 *
	 * @returns boolean True when the texture is loaded.
	 */
	int Texture_is_loaded(lua_State *lua)
	{
		Texture *texture = castUData<Texture>(lua, 1);
		if (texture)
		{
			lua_pushboolean(lua, texture->isLoaded());
			return 1;
		}
		return LuaState::expectedContext(lua, "is_loaded", "am.texture");
	}

	/**
	 * Attempts to reload the texture from its currently set filename.
	 *
	 * @returns am.code Return codes:
	 * <table class='return_codes'>
	 *  <tr><td>success</td><td>The texture loaded the new file successfully.</td></tr>
	 *  <tr><td>file_not_found</td><td>Texture didn't load because the file could not be found.</td></tr>
	 *  <tr><td>texture_load_failed</td><td>Texture didn't load because there was an issue with the file.</td></tr>
	 * </table>
	 */
	int Texture_reload(lua_State *lua)
	{
		Texture *texture = castUData<Texture>(lua, 1);
		if (texture)
		{
			lua_pushinteger(lua, texture->reload());
			return 1;
		}
		return LuaState::expectedContext(lua, "reload", "am.texture");
	}

	/**
	 * Returns the pixel width of the texture. Returns 0 if the texture isn't loaded.
	 *
	 * @returns integer The width of the texture.
	 */
	int Texture_width(lua_State *lua)
	{
		Texture *texture = castUData<Texture>(lua, 1);
		if (texture)
		{
			lua_pushinteger(lua, texture->getWidth());
			return 1;
		}
		return LuaState::expectedContext(lua, "width", "am.texture");
	}

	/**
	 * Returns the pixel height of the texture. Returns 0 if the texture isn't loaded.
	 *
	 * @returns integer The height of the texture.
	 */
	int Texture_height(lua_State *lua)
	{
		Texture *texture = castUData<Texture>(lua, 1);
		if (texture)
		{
			lua_pushinteger(lua, texture->getHeight());
			return 1;
		}
		return LuaState::expectedContext(lua, "height", "am.texture");
	}

	/**
	 * Returns the number of bytes per pixel. Returns 0 if the texture isn't loaded.
	 *
	 * @returns integer The bytes per pixel of the texture.
	 */
	int Texture_bytes_per_pixel(lua_State *lua)
	{
		Texture *texture = castUData<Texture>(lua, 1);
		if (texture)
		{
			lua_pushinteger(lua, texture->getBytesPerPixel());
			return 1;
		}
		return LuaState::expectedContext(lua, "bytes_per_pixel", "am.texture");
	}

	/**
	 * Returns the OpenGL format. Returns 0 if the texture isn't loaded.
	 * Not likely to be useful in many cases but it's here.
	 *
	 * @returns integer The bytes per pixel of the texture.
	 */
	int Texture_gl_format(lua_State *lua)
	{
		Texture *texture = castUData<Texture>(lua, 1);
		if (texture)
		{
			lua_pushinteger(lua, texture->getGlFormat());
			return 1;
		}
		return LuaState::expectedContext(lua, "gl_format", "am.texture");
	}
}
}
}
