#include "lua_sound.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <sfx/sfx_engine.h>
#include <sfx/sfx_isound.h>
using namespace am::sfx;

namespace am {
namespace lua {
namespace sfx {
	/**
	 * @class
	 * Simple wrapper for the sound class, currently only allows for loading
	 * a sound.
	 */
	/**
	 * Creates a new sound from a filename as either a whole sound or a streaming sound.
	 *
	 * @param string filename The path to the sound file.
	 * @param boolean [false] to_stream When true the sound will be loaded for streaming
	 *  which is best for long music tracks. However streaming sounds can only be attached
	 *  to one sound source. Non-streaming sounds cna be attached to as many sound sources
	 *  as you like.
	 */
	int Sound_ctor(lua_State *lua)
	{
		if (lua_gettop(lua) == 1)
		{
			bool isStream = lua_tobool(lua, 2);
			if (lua_isstr(lua, 1))
			{
				ISound *sound = nullptr;
				if (isStream)
				{
					sound = SfxEngine::getEngine()->loadStream(lua_tostring(lua, 1));
				}
				else
				{
					sound = SfxEngine::getEngine()->loadSound(lua_tostring(lua, 1));
				}
				if (sound)
				{
					wrapRefObject<ISound>(lua, sound);
					return 1;
				}
				lua_pushnil(lua);
				return 1;
			}
		}
		return LuaState::expectedArgs(lua, "@new", "string filename, bool [false] to_stream");
	}
	/**
	 * Releases the reference counter on the sound.
	 */
	int Sound_dtor(lua_State *lua)
	{
		ISound *sound = castUData<ISound>(lua, 1);
		if (sound)
		{
			sound->release();
		}
		return 0;
	}
	/**
	 * Compares this sound with another sound object.
	 *
	 * @param am.sound rhs The other sound to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int Sound_eq(lua_State *lua)
	{
		ISound *lhs = castUData<ISound>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.sound");
		}
		ISound *rhs = castUData<ISound>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Sound_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Sound_ctor },
			{ "__gc", Sound_dtor },
			{ "__eq", Sound_eq },
			{ "filename", Sound_filename },
			{ "streaming", Sound_streaming },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, ISound::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	/**
	 * Returns the filename of the loaded sound. An empty string indicated
	 * an unloaded sound.
	 *
	 * @returns string The file name of the loaded sound.
	 */
	int Sound_filename(lua_State *lua)
	{
		ISound *sound = castUData<ISound>(lua, 1);
		if (sound)
		{
			lua_pushstring(lua, sound->getFilename());
			return 1;
		}
		return LuaState::expectedContext(lua, "filename", "am.sound");
	}

	/**
	 * Returns true if the sound is a streaming sound.
	 *
	 * @returns boolean True if the sound is being streamed.
	 */
	int Sound_streaming(lua_State *lua)
	{
		ISound *sound = castUData<ISound>(lua, 1);
		if (sound)
		{
			lua_pushboolean(lua, sound->isStream());
			return 1;
		}
		return LuaState::expectedContext(lua, "streaming", "am.sound");
	}
	
}
}
}
