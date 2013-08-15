#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace gfx {
	class Asset;
}

namespace lua {
namespace gfx {

	int Asset_ctor(lua_State *lua);
	int Asset_dtor(lua_State *lua);
	int Asset_eq(lua_State *lua);
	int Asset_register(lua_State *lua);

	int Asset_texture(lua_State *lua);
	int Asset_add_texture(lua_State *lua);
	int Asset_remove_texture(lua_State *lua);
	int Asset_has_texture(lua_State *lua);
	int Asset_total_textures(lua_State *lua);
	int Asset_total_texture_windows(lua_State *lua);
	int Asset_texture_window(lua_State *lua);
	int Asset_texture_window_pixel(lua_State *lua);

	int Asset_num_frames(lua_State *lua);
	int Asset_frame_rate(lua_State *lua);
	int Asset_width(lua_State *lua);
	int Asset_height(lua_State *lua);
	int Asset_repeat(lua_State *lua);

	int Asset_scale_nine(lua_State *lua);
	int Asset_scale_nine_state(lua_State *lua);
	int Asset_texture_window_animation(lua_State *lua);
}
}
}
