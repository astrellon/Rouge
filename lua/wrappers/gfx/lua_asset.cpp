#include "lua_asset.h"

extern "C" 
{ 
#	include <lua/src/lua.h>
#	include <lua/src/lauxlib.h>
#	include <lua/src/lualib.h>
}

#include <lua/lua_state.h>
using namespace am::lua;

#include <gfx/gfx_texture.h>
#include <gfx/gfx_asset.h>
#include <gfx/gfx_engine.h>
using namespace am::gfx;

#include <math/math.h>
using namespace am;

namespace am {
namespace lua {
namespace gfx {
	/**
	 * @class
	 * Simple wrapper for the asset class.
	 */
	/**
	 * @static
	 * Creates a new asset using the given asset name.
	 *
	 * @param string asset_name Name of the asset.
	 */
	int Asset_ctor(lua_State *lua)
	{
		if (lua_gettop(lua) == 1)
		{
			if (lua_isstr(lua, 1))
			{
				Asset *asset = new Asset(lua_tostring(lua, 1));
				wrapRefObject<Asset>(lua, asset);
				return 1;
			}
		}
		return LuaState::expectedArgs(lua, "@new", "string asset_name");
	}
	/**
	 * Releases the reference counter on the asset.
	 */
	int Asset_dtor(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			asset->release();
		}
		return 0;
	}
	/**
	 * Compares this asset with another asset object.
	 *
	 * @param am.asset rhs The other sprite to compare with.
	 * @returns boolean True if they are the same object.
	 */
	int Asset_eq(lua_State *lua)
	{
		Asset *lhs = castUData<Asset>(lua, 1);
		if (!lhs)
		{
			return LuaState::expectedContext(lua, "__eq", "am.asset");
		}
		Asset *rhs = castUData<Asset>(lua, 2);
		lua_pushboolean(lua, lhs == rhs);
		return 1;
	}

	int Asset_register(lua_State *lua)
	{
		luaL_Reg regs[] = 
		{
			{ "new", Asset_ctor },
			{ "__gc", Asset_dtor },
			{ "__eq", Asset_eq },
			{ "texture", Asset_texture },
			{ "add_texture", Asset_add_texture },
			{ "remove_texture", Asset_remove_texture },
			{ "has_texture", Asset_has_texture },
			{ "total_textures", Asset_total_textures },
			{ "total_texture_windows", Asset_total_texture_windows },
			{ "texture_window", Asset_texture_window },
			{ "texture_window_pixel", Asset_texture_window_pixel },
			{ "num_frames", Asset_num_frames },
			{ "frame_rate", Asset_frame_rate },
			{ "width", Asset_width },
			{ "height", Asset_height },
			{ "repeat", Asset_repeat },
			{ "scale_nine", Asset_scale_nine },
			{ "scale_nine_state", Asset_scale_nine_state },
			{ "texture_window_animation", Asset_texture_window_animation },
			{ nullptr, nullptr }
		};

		luaL_newmetatable(lua, Asset::LUA_TABLENAME);
		luaL_setfuncs(lua, regs, 0);

		lua_pushvalue(lua, -1);
		lua_setfield(lua, -1, "__index");

		return 1;
	}
	
	/**
	 * Returns the texture at the given index. Defaults to getting the first texture.
	 *
	 * @param integer [0] index The index of the texture to get.
	 * @returns am.texture The found texture or nil if there is no texture at that index.
	 */
	/**
	 * Sets the texture at the given index. Defaults to setting the first texture.
	 *
	 * @param am.texture texture The texture to set.
	 * @param integer [0] index The index to set the texture at.
	 * @returns am.asset This
	 * @returns am.code Success.
	 */
	/**
	 * Sets the texture by filename at the given index. 
	 * Defaults to setting the first texture.
	 *
	 * @param string filename The path to the texture.
	 * @param integer [0] index The index to set the texture at.
	 * @returns am.asset This
	 * @returns am.code Return code for loading the texture.
	 * <table class='return_codes'>
	 *  <tr><td>success</td><td>The texture loaded the new file successfully.</td></tr>
	 *  <tr><td>file_not_found</td><td>Texture didn't load because the file could not be found.</td></tr>
	 *  <tr><td>texture_load_failed</td><td>Texture didn't load because there was an issue with the file.</td></tr>
	 * </table>
	 */
	int Asset_texture(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			int args = lua_gettop(lua);
			// Returns the first texture
			if (args == 1)
			{
				Handle<Texture> texture = asset->getTexture();
				if (texture)
				{
					wrapRefObject<Texture>(lua, texture);
					return 1;
				}
				lua_pushnil(lua);
				return 1;
			}
			else if (args == 2)
			{
				// Returns the texture at the index.
				if (lua_isnum(lua, 2))
				{
					Handle<Texture> texture = asset->getTexture(lua_tointeger(lua, 2));
					if (texture)
					{
						wrapRefObject<Texture>(lua, texture);
						return 1;
					}
					lua_pushnil(lua);
					return 1;
				}
				Handle<Texture> texture;
				if (lua_isstr(lua, 2))
				{
					ReturnCode result = GfxEngine::getEngine()->getTexture(lua_tostring(lua, 2), texture);
					if (result != SUCCESS)
					{
						lua_pushvalue(lua, 1);
						lua_pushinteger(lua, result);
						return 2;
					}
				}
				else
				{
					texture = castUData<Texture>(lua, 2);
				}
				if (texture)
				{
					unsigned int index = 0;
					if (lua_isnum(lua, 3))
					{
						index = static_cast<unsigned int>(lua_tointeger(lua, 3));
					}
					asset->setTexture(texture, index);
					lua_pushvalue(lua, 1);
					lua_pushinteger(lua, SUCCESS);
					return 2;
				}
			}
			return LuaState::expectedArgs(lua, "texture", 2, "integer index = 0", "am.texture texture, integer index = 0");
		}
		return LuaState::expectedContext(lua, "texture", "am.asset");
	}

	/**
	 * Adds a texture to the list of texture on this asset.
	 *
	 * @param am.texture texture The texture to add.
	 * @returns am.asset This
	 * @returns am.code Success
	 */
	/**
	 * Attempts to add a texture to the list of textures on this asset by texture name.
	 * 
	 * @param string filename The path to load the texture from.
	 * @returns am.asset This
	 * @returns am.code Return code for loading the texture.
	 * <table class='return_codes'>
	 *  <tr><td>success</td><td>The texture loaded the new file successfully.</td></tr>
	 *  <tr><td>file_not_found</td><td>Texture didn't load because the file could not be found.</td></tr>
	 *  <tr><td>texture_load_failed</td><td>Texture didn't load because there was an issue with the file.</td></tr>
	 * </table>
	 */
	int Asset_add_texture(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			if (lua_isstr(lua, 2))
			{
				Texture *texture = nullptr;
				ReturnCode result = GfxEngine::getEngine()->getTexture(lua_tostring(lua, 2), texture);
				if (result != SUCCESS)
				{
					lua_pushvalue(lua, 1);
					lua_pushinteger(lua, result);
					return 2;
				}
				asset->addTexture(texture);
				lua_first(lua);
			}
			Texture *texture = castUData<Texture>(lua, 2);
			if (texture)
			{
				asset->addTexture(texture);
				lua_pushvalue(lua, 1);
				lua_pushinteger(lua, SUCCESS);
				return 2;
			}
			return LuaState::expectedArgs(lua, "add_texture", 2, "am.texture texture", "string filename");
		}
		return LuaState::expectedContext(lua, "add_texture", "am.asset");
	}

	/**
	 * Removes the texture based off the filename used to load that texture.
	 *
	 * @param string filename The filename the texture was loaded from.
	 * @returns am.asset This
	 */
	/**
	 * Removes the given texture from the asset list.
	 *
	 * @param am.texture texture The texture to remove.
	 * @returns am.asset This
	 */
	int Asset_remove_texture(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			if (lua_isstr(lua, 2))
			{
				asset->removeTexture(lua_tostring(lua, 2);
				lua_first(lua);
			}
			Texture *texture = castUData<Texture>(lua, 2);
			if (texture)
			{
				asset->removeTexture(texture);
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "remove_texture", 2, "am.texture texture", "string filename");
		}
		return LuaState::expectedContext(lua, "remove_texture", "am.asset");
	}

	/**
	 * Check if the asset contains the texture based off the filename used to load that texture.
	 *
	 * @param string filename The filename the texture was loaded from.
	 * @returns boolean True if there was a texture found that used the given filename.
	 */
	/**
	 * Checks if the asset contains the given texture.
	 *
	 * @param am.texture texture The texture to check for.
	 * @returns boolean True if the texture was found.
	 */
	int Asset_has_texture(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			if (lua_isstr(lua, 2))
			{
				lua_pushboolean(lua, asset->hasTexture(lua_tostring(lua, 2));
				return 1;
			}
			Texture *texture = castUData<Texture>(lua, 2);
			if (texture)
			{
				lua_pushboolean(lua, asset->hasTexture(texture));
				return 1;
			}
			return LuaState::expectedArgs(lua, "has_texture", 2, "am.texture texture", "string filename");
		}
		return LuaState::expectedContext(lua, "has_texture", "am.asset");
	}

	/**
	 * Returns the number of textures associated with this asset.
	 *
	 * @returns integer The number of textures.
	 */
	int Asset_total_textures(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			lua_pushinteger(lua, asset->getTotalTextures());
			return 1;
		}
		return LuaState::expectedContext(lua, "total_textures", "am.asset");
	}

	/**
	 * Returns the number of texture windows this asset can use.
	 *
	 * @returns integer The number of texture windows.
	 */
	/**
	 * Sets the number of texture windows this asset will use.
	 * This is related to the number of frames set (see num_frames).
	 * For example is the number of frames is set to 4, 4, then the maximum number of
	 * frames the texture can be split up into is 16 and by default the total number of texture
	 * windows is set to 16, however if the graphic is only made up of 14 frames but still in a 4x4 grid
	 * then you will need to set this to 14.
	 *
	 * @param integer total_texture_windows The total number of texture windows to use.
	 * @returns am.asset This.
	 */
	int Asset_total_texture_windows(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushinteger(lua, asset->getTotalSubWindows());
				return 1;
			}
			else if (lua_isnum(lua, 2))
			{
				asset->setTotalSubWindows(lua_tointeger(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "total_texture_windows", "integer total_texture_windows");
		}
		return LuaState::expectedContext(lua, "total_texture_windows", "am.asset");
	}

	/**
	 * Returns a table with values that correspond with the UV coordinates that
	 * defines the subwindow within the texture.
	 * This subwindow allows a section of a texture to be used instead of the whole texture.
	 *
	 * The table contains the following values: "left_x", "right_x", "top_y" and "bottom_y"
	 *
	 * @returns table Table containing the UV coordinates.
	 */
	/**
	 * Sets the texture window rectangle for this asset from a table.
	 * Each value should be treated like a UV coordinate.
	 *
	 * The table is expected to have the values for "left_x", "right_x", "top_y" and "bottom_y".
	 * If these values are not defined, left_x and top_y default to 0 and right_x and bottom_y will default to 1.
	 * 
	 * @param table values Table of values to set the UV coordinates to.
	 * @returns am.asset This
	 */
	int Asset_texture_window(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			if (lua_gettop(lua) == 1)
			{
				const TextureWindow &win = asset->getTextureWindow();
				lua_createtable(lua, 0, 4);
				LuaState L(lua);
				L.setTableValue("left_x", win.getLeftX());
				L.setTableValue("right_x", win.getRightX());
				L.setTableValue("top_y", win.getTopY());
				L.setTableValue("bottom_y", win.getBottomY());
				return 1;
			}
			else if (lua_istable(lua, 2))
			{
				LuaState L(lua);
				double leftX = 0.0, rightX = 1.0, topY = 0.0, bottomY = 1.0, width = -1.0, height = -1.0;
				L.getTableDouble("left_x", leftX);
				L.getTableDouble("right_x", rightX);
				L.getTableDouble("top_y", topY);
				L.getTableDouble("bottom_y", bottomY);
				
				TextureWindow win;
				win.setValues(topY, bottomY, leftX, rightX);
				asset->setTextureWindow(win);
			}
			return LuaState::expectedArgs(lua, "texture_window", "table values");
		}
		return LuaState::expectedContext(lua, "texture_window", "am.asset");
	}

	/**
	 * Returns a table of values that correspond with the pixel coordinates that
	 * define the subwindow within the texture.
	 * This subwindow allows a section of a texture to be used instead of the whole texture.
	 * This function will fail if a texture has not been set onto the asset.
	 * We may lift this restriction in future if it is required.
	 *
	 * The table contains the following values: "left_x", "right_x", "top_y" and "bottom_y"
	 *
	 * @returns table x The x position of the rectangle
	 */
	/**
	 * Sets the texture window rectangle for this asset from a table.
	 * Each value should be treated like a pixel coordinate.
	 * This function will fail if a texture has not been set onto the asset.
	 * We may lift this restriction in future if it is required.
	 *
	 * The table is expected to have the values for "left_x", "right_x", "top_y" and "bottom_y".
	 * If these values are not defined, left_x and top_y default to 0 and right_x and bottom_y will default to the texture width.
	 * 
	 * @param table values A table of values defining the texture window in pixel coordinates.
	 * @returns am.asset This
	 */
	int Asset_texture_window_pixel(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			if (lua_gettop(lua) == 1)
			{
				const TextureWindow &win = asset->getTextureWindow();
				Handle<const Texture> texture(asset->getTexture());
				if (!texture)
				{
					LuaState::warning(lua, "Unable to get pixel texture coordinates from an asset that has no texture defined.");
					lua_pushnil(lua);
					return 1;
				}
				lua_createtable(lua, 0, 4);
				LuaState L(lua);
				L.setTableValue("left_x", win.getLeftX() * static_cast<float>(texture->getWidth()));
				L.setTableValue("right_x", win.getRightX() * static_cast<float>(texture->getWidth()));
				L.setTableValue("top_y", win.getTopY() * static_cast<float>(texture->getHeight()));
				L.setTableValue("bottom_y", win.getBottomY() * static_cast<float>(texture->getHeight()));
				return 1;
			}
			else if (lua_istable(lua, 2))
			{
				LuaState L(lua);
				Handle<const Texture> texture(asset->getTexture());
				if (texture)
				{
					double width = static_cast<double>(texture->getWidth()), height = static_cast<double>(texture->getHeight());
					double leftX = 0.0, rightX = width, topY = 0.0, bottomY = height;
					
					L.getTableDouble("left_x", leftX);
					L.getTableDouble("right_x", rightX);
					L.getTableDouble("top_y", topY);
					L.getTableDouble("bottom_y", bottomY);

					TextureWindow win;
					win.setValues(math::abs(rightX - leftX), math::abs(bottomY - topY), 
						topY / height, bottomY / height, 
						leftX / width, rightX / width);
					asset->setTextureWindow(win);
				}
				else
				{
					LuaState::warning(lua, "Unable to set pixel texture coordinates onto asset that has not texture.");
				}
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "texture_window_pixel", "table values");
		}
		return LuaState::expectedContext(lua, "texture_window_pixel", "am.asset");
	}

	/**
	 * Returns the number of frames the asset has in the x and y directions.
	 * These frames are in relation to the assets texture window.
	 *
	 * @returns integer The number of x frames.
	 * @returns integer The number of y frames.
	 */
	/**
	 * Sets the number of frames the asset has in the x and y directions.
	 * These frames are in relation to the assets texture window.
	 *
	 * @param integer frame_x The number of x frames.
	 * @param integer frame_y The number of y frames.
	 * @returns am.asset This
	 */
	int Asset_num_frames(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			int args = lua_gettop(lua);
			if (args == 1)
			{
				lua_pushinteger(lua, asset->getNumFramesX());
				lua_pushinteger(lua, asset->getNumFramesY());
				return 2;
			}
			else if (lua_isnum(lua, 2) && (args == 2 || (args == 3 && lua_isnum(lua, 3))))
			{
				asset->setNumFramesX(lua_tointeger(lua, 2));
				if (args == 3)
				{
					asset->setNumFramesY(lua_tointeger(lua, 3));
				}
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "num_frames", 2, "integer frames_x", "integer frames_x, integer frames_y");
		}
		return LuaState::expectedContext(lua, "num_frames", "am.asset");
	}

	/**
	 * Returns the frame rate of the asset graphic. The defines how
	 * quickly the animation progresses. If the framerate is set to zero (or below)
	 * it is assumed that this asset has no animation.
	 * The frame rate is defined in frames per second.
	 * See subwindow_animation for types of animation.
	 *
	 * @returns number The frame rate.
	 */
	/**
	 * Sets the frame rate of the asset.
	 * The frame rate is defined as how many frames per second.
	 * See subwindow_animation for types of animation.
	 *
	 * @param number frame_rate The frame rate.
	 * @returns am.asset This
	 */
	int Asset_frame_rate(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushnumber(lua, asset->getFrameRate());
				return 1;
			}
			else if (lua_isnum(lua, 2))
			{
				asset->setFrameRate(lua_tofloat(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "frame_rate", "number frame_rate");
		}
		return LuaState::expectedContext(lua, "frame_rate", "am.asset");
	}

	/**
	 * Returns the width of the asset in pixels. This takes into account the subwindow size and the number
	 * of frames.
	 *
	 * @returns number The width of the asset graphic.
	 */
	int Asset_width(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			lua_pushnumber(lua, asset->getWidth());
			return 1;
		}
		return LuaState::expectedContext(lua, "width", "am.asset");
	}
	/**
	 * Returns the height of the asset in pixels. This takes into account the subwindow size and the number
	 * of frames.
	 *
	 * @returns number The height of the asset graphic.
	 */
	int Asset_height(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			lua_pushnumber(lua, asset->getHeight());
			return 1;
		}
		return LuaState::expectedContext(lua, "height", "am.asset");
	}

	/**
	 * Returns the flags for how the asset textures are repeated in the x and y directions.
	 *
	 * @returns boolean If the texture will repeat along the x direction.
	 * @returns boolean If the texture will repeat along the y direction.
	 */
	/**
	 * Sets the flags for how the texture repreats.
	 *
	 * @param boolean repeat_x If the texture will repeat along the x direction.
	 * @param boolean repeat_y If the texture will repeat along the y direction.
	 * @returns am.asset This
	 */
	int Asset_repeat(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			int args = lua_gettop(lua);
			if (args == 1)
			{
				lua_pushboolean(lua, asset->getRepeatX());
				lua_pushboolean(lua, asset->getRepeatY());
				return 2;
			}
			else if (lua_isbool(lua, 2) && (args == 2 || (args == 3 && lua_isbool(lua, 3))))
			{
				asset->setRepeatX(lua_tobool(lua, 2));
				if (args == 3)
				{
					asset->setRepeatY(lua_tobool(lua, 3));
				}
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "repeat", 2, "boolean repeat_x", "boolean repeat_x, boolean repeat_y");
		}
		return LuaState::expectedContext(lua, "repeat", "am.asset");
	}

	/**
	 * Returns a table with values that define the inner borders of a scale nine system.
	 * Scale nine allows for an asset to be stretched only whilst keep the outer edge unstretched.
	 *
	 * The table contains the following values: "left_x", "right_x", "top_y" and "bottom_y"
	 *
	 * @returns table A table that defines the inner borders of a scale nine system in pixels.
	 */
	/**
	 * Sets the inner borders of a scale nine system.
	 * Scale nine allows for an asset to be stretched only whilst keep the outer edge unstretched.
	 *
	 * The table is expected to have the values for "left_x", "right_x", "top_y" and "bottom_y".
	 * If these values are not defined, the values are left unset.
	 *
	 * These values are used in connection with the scale nine state, which defines if the scale nine
	 * system is to be turned off or used in horizontal, verticle or both modes. When automatically
	 * calculated the state it determined by which values are defined, if only left_x and right_x are set
	 * then it's determined to be horizontal, same with top_y and bottom_y. If all are set then 
	 * it's set to both. And if none are set then scale nine is turned off.
	 * 
	 * @param table values A table of values defining the scale nine inner borders in pixels.
	 * @param boolean [false] calc_state When true the scale nine state will be automatically calculated from the values given.
	 * @returns am.asset This
	 */
	/**
	 * Sets the inner borders of a scale nine system.
	 * Scale nine allows for an asset to be stretched only whilst keep the outer edge unstretched.
	 *
	 * The table is expected to have the values for "left_x", "right_x", "top_y" and "bottom_y".
	 * If these values are not defined, the values are left unset.
	 *
	 * These values are used in connection with the scale nine state, which defines if the scale nine
	 * system is to be turned off or used in horizontal, verticle or both modes.
	 * The valid values for setting the scale nine state are "none", "verticle", "horizontal" and "both".
	 * 
	 * @param table values A table of values defining the scale nine inner borders in pixels.
	 * @param string state The scale nine state to set.
	 * @returns am.asset This
	 */
	int Asset_scale_nine(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			if (lua_gettop(lua) == 1)
			{
				const ScaleNine &scaleNine = asset->getScaleNine();
				lua_createtable(lua, 0, 4);
				LuaState L(lua);
				L.setTableValue("left_x", scaleNine.getLeft());
				L.setTableValue("right_x", scaleNine.getRight());
				L.setTableValue("top_y", scaleNine.getTop());
				L.setTableValue("bottom_y", scaleNine.getBottom());
				return 1;
			}
			else if (lua_istable(lua, 2))
			{
				LuaState L(lua);
				double leftX = -1.0, rightX = -1.0, topY = -1.0, bottomY = -1.0;
					
				L.getTableDouble("left_x", leftX);
				L.getTableDouble("right_x", rightX);
				L.getTableDouble("top_y", topY);
				L.getTableDouble("bottom_y", bottomY);
				ScaleNine scaleNine;
				scaleNine.setInnerBounds(leftX, rightX, topY, bottomY);
				if (lua_isbool(lua, 3) && lua_tobool(lua, 3))
				{
					bool verticle = topY >= 0.0 && bottomY > 0.0;
					bool horizontal = leftX >= 0.0 && rightX > 0.0;
					ScaleNineState state = SCALE_NINE_NONE;
					if (verticle && !horizontal)
					{
						state = SCALE_NINE_VERTICAL;
					}
					else if (!verticle && horizontal)
					{
						state = SCALE_NINE_HORIZONTAL;
					}
					else if (verticle && horizontal)
					{
						state = SCALE_NINE_BOTH;
					}
					asset->setScaleNine(scaleNine, state);
				}
				else if (lua_isstr(lua, 3))
				{
					ScaleNineState state = ScaleNine::getState(lua_tostring(lua, 3));
					asset->setScaleNine(scaleNine, state);
				}
				else
				{
					asset->setScaleNine(scaleNine);
				}
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "scale_nine", 2, "table values, boolean [false] calc_state", "table values, string state");
		}
		return LuaState::expectedContext(lua, "scale_nine", "am.asset");
	}

	/**
	 * Returns the scale nine state. This state is used to determine how the scale nine inner borders are used.
	 * 
	 * Can return the following values: "none", "horizontal", "verticle" or "both"
	 *
	 * @returns string The scale nine state.
	 */
	/**
	 * Sets the scale nine state by name. This state is used to determine how the scale nine inner borders are used.
	 * 
	 * <table class='return_codes'>
	 *  <tr><td>none</td><td>Scale nine is turned off.</td></tr>
	 *  <tr><td>horizontal</td><td>Only the left_x and right_x values are used and the asset is stretched horizontally.</td></tr>
	 *  <tr><td>vertical</td><td>Only the top_y and bottom_y values are used and the asset is stretched vertically.</td></tr>
	 *  <tr><td>both</td><td>All 4 values are used and the asset can be stretched both horizontally and vertically.</td></tr>
	 * </table>
	 * 
	 * @param string state The state to set the scale nine system to.
	 * @returns am.asset This.
	 */
	int Asset_scale_nine_state(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushstring(lua, ScaleNine::getStateName(asset->getScaleNineState());
				return 1;
			}
			else if (lua_isstr(lua, 2))
			{
				ScaleNineState state = ScaleNine::getState(lua_tostring(lua, 2));
				asset->setScaleNineState(state);
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "scale_nine_state", "string state");
		}
		return LuaState::expectedContext(lua, "scale_nine_state", "am.asset");
	}

	/**
	 * Returns if the animation for this asset is based on texture windows.
	 * 
	 * @returns boolean True if this asset animates along the texture windows.
	 */
	/**
	 * Sets how the asset deals with animation. For assets that have a frame rate of zero this value is ignored.
	 *
	 * When true, the current frame defines which texture window is used to display. So for an asset with
	 * num_frames set to 4, 4, it will have a total of 16 sub texture windows to animate over.
	 *
	 * When false the current frame defines which texture out of the list of all textures added to this asset
	 * to use when animating.
	 *
	 * @param boolean texture_window_animation The type of animation for this asset.
	 * @returns am.asset This.
	 */
	int Asset_texture_window_animation(lua_State *lua)
	{
		Asset *asset = castUData<Asset>(lua, 1);
		if (asset)
		{
			if (lua_gettop(lua) == 1)
			{
				lua_pushboolean(lua, asset->isSubWindowAnimation());
				return 1;
			}
			else if (lua_isbool(lua, 2))
			{
				asset->setSubWindowAnimation(lua_tobool(lua, 2));
				lua_first(lua);
			}
			return LuaState::expectedArgs(lua, "texture_window_animation", "boolean animation");
		}
		return LuaState::expectedContext(lua, "texture_window_animation", "am.asset");
	}

}
}
}
