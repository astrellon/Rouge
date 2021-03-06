#include <ui/lua_event_listener.h>

#include <lua/wrappers/game/lua_item.h>
#include <lua/wrappers/game/lua_inventory.h>
#include <lua/wrappers/game/lua_character.h>
#include <lua/wrappers/game/lua_dialogue.h>
#include <lua/wrappers/game/lua_game.h>

#include <sstream>

#include <game/character.h>
#include <game/inventory.h>

namespace am {
namespace ui {

	LuaEventListener::LuaEventListener(lua_State *lua, int funcRef, int contextRef) :
		mLua(lua),
		mFuncRef(funcRef),
		mContextRef(contextRef)
	{

	}
	LuaEventListener::~LuaEventListener()
	{
        luaL_unref(mLua, LUA_REGISTRYINDEX, mFuncRef);
        luaL_unref(mLua, LUA_REGISTRYINDEX, mContextRef);
	}

	lua::LuaState &LuaEventListener::getLua()
	{
		return mLua;
	}

	int LuaEventListener::getFuncRef() const
	{
		return mFuncRef;
	}
	int LuaEventListener::getContextRef() const
	{
		return mContextRef;
	}

	void LuaEventListener::onEvent(Event *e)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, mFuncRef);
		bool contexted = mContextRef != LUA_REFNIL;
		if (contexted)
		{
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, mContextRef);
		}
		mLua.newTable();
		mLua.setTableValue("type", e->getType().c_str());
		lua_acall(mLua, contexted ? 2 : 1, 0);
	}
	void LuaEventListener::onEvent(MouseEvent *e)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, mFuncRef);
		bool contexted = mContextRef != LUA_REFNIL;
		if (contexted)
		{
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, mContextRef);
		}
		mLua.newTable();
		mLua.setTableValue("type", e->getType().c_str());
		mLua.setTableValue("local_x", e->getLocalMouseX());
		mLua.setTableValue("local_y", e->getLocalMouseY());
		mLua.setTableValue("mouse_button", static_cast<int>(e->getMouseButton()));
		mLua.setTableValue("mouse_x", e->getMouseX());
		mLua.setTableValue("mouse_y", e->getMouseY());
		lua_acall(mLua, contexted ? 2 : 1, 0);
	}
	void LuaEventListener::onEvent(KeyboardEvent *e)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, mFuncRef);
		bool contexted = mContextRef != LUA_REFNIL;
		if (contexted)
		{
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, mContextRef);
		}
		mLua.newTable();
		mLua.setTableValue("type", e->getType().c_str());
		mLua.setTableValue("key", e->getKey());
		mLua.setTableValue("is_system_key", e->isSystemKey());
		lua_acall(mLua, contexted ? 2 : 1, 0);
	}
	void LuaEventListener::onEvent(InventoryEvent *e)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, mFuncRef);
		bool contexted = mContextRef != LUA_REFNIL;
		if (contexted)
		{
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, mContextRef);
		}
		mLua.newTable();
		mLua.setTableValue("type", e->getType().c_str());
		mLua.setTableValue("spot_x", e->getSpotX());
		mLua.setTableValue("spot_y", e->getSpotY());
		mLua.push("item");
		lua::wrapRefObject<game::Item>(mLua, e->getItem());
		lua_settable(mLua, -3);
		mLua.push("inventory");
		lua::wrapRefObject<game::Inventory>(mLua, e->getInventory());
		lua_settable(mLua, -3);
		lua_acall(mLua, contexted ? 2 : 1, 0);
	}
	void LuaEventListener::onEvent(EquipEvent *e)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, mFuncRef);
		bool contexted = mContextRef != LUA_REFNIL;
		if (contexted)
		{
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, mContextRef);
		}
		mLua.newTable();
		mLua.setTableValue("type", e->getType().c_str());
		mLua.push("body_part");
		lua::wrapRefObject<game::BodyPart>(mLua, e->getBodyPart());
		lua_settable(mLua, -3);
		mLua.push("item");
		if (e->getItem())
		{
			lua::wrapRefObject<Item>(mLua, e->getItem());
		}
		else
		{
			lua_pushnil(mLua);
		}
		lua_settable(mLua, -3);
		mLua.push("character");
		if (e->getCharacter())
		{
			lua::wrapRefObject<game::Character>(mLua, e->getCharacter());
		}
		else
		{
			lua_pushnil(mLua);
		}
		lua_settable(mLua, -3);
		lua_acall(mLua, contexted ? 2 : 1, 0);
	}
	void LuaEventListener::onEvent(DialogueEvent *e)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, mFuncRef);
		bool contexted = mContextRef != LUA_REFNIL;
		if (contexted)
		{
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, mContextRef);
		}
		mLua.newTable();
		mLua.setTableValue("type", e->getType().c_str());
		// Talked To
		mLua.push("talked_to");
		lua::game::wrapGameObject(mLua, e->getTalkedTo());
		lua_settable(mLua, -3);
		// Talker
		mLua.push("talker");
		lua::game::wrapGameObject(mLua, e->getTalker());
		lua_settable(mLua, -3);
		// Dialogue
		mLua.push("dialogue");
		Dialogue *diag = e->getDialogue();
		if (diag)
		{
			lua::wrapObject<game::Dialogue>(mLua, diag);
		}
		else
		{
			lua_pushnil(mLua);
		}
		lua_settable(mLua, -3);

		lua_acall(mLua, contexted ? 2 : 1, 0);
	}
	void LuaEventListener::onEvent(ItemEvent *e)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, mFuncRef);
		bool contexted = mContextRef != LUA_REFNIL;
		if (contexted)
		{
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, mContextRef);
		}
		mLua.newTable();
		mLua.setTableValue("type", e->getType().c_str());
		// Item
		mLua.push("item");
		if (e->getItem())
		{
			lua::wrapRefObject<game::Item>(mLua, e->getItem());
		}
		else
		{
			lua_pushnil(mLua);
		}
		lua_settable(mLua, -3);
		// Extra
		mLua.push("extra");
		lua::game::wrapGameObject(mLua, e->getExtra());
		lua_settable(mLua, -3);
		
		lua_acall(mLua, contexted ? 2 : 1, 0);
	}
	void LuaEventListener::onEvent(MapRegionEvent *e)
	{
		lua_rawgeti(mLua, LUA_REGISTRYINDEX, mFuncRef);
		bool contexted = mContextRef != LUA_REFNIL;
		if (contexted)
		{
			lua_rawgeti(mLua, LUA_REGISTRYINDEX, mContextRef);
		}
		mLua.newTable();
		mLua.setTableValue("type", e->getType().c_str());

		mLua.push("game_object");
		lua::game::wrapGameObject(mLua, e->getGameObject());
		lua_settable(mLua, -3);

		mLua.push("map_region");
		lua::wrapRefObject<game::MapRegion>(mLua, e->getMapRegion());
		lua_settable(mLua, -3);

		lua_acall(mLua, contexted ? 2 : 1, 0);
	}

	bool LuaEventListener::operator==(const LuaEventListener *rhs) const
	{
		return compareListeners(rhs);
	}

	bool LuaEventListener::operator==(const IEventListener *rhs) const
	{
		const LuaEventListener *rhsLua = dynamic_cast<const LuaEventListener *>(rhs);
		if (rhsLua)
		{
			return compareListeners(rhsLua);
		}
		return false;
	}

	bool LuaEventListener::compareListeners(const LuaEventListener *rhs) const
	{
		if (rhs == nullptr)
		{
			return false;
		}
		if (rhs->mLua != mLua)
		{
			return false;
		}
		return mLua.compareRefs(mContextRef, rhs->mContextRef) &&
			mLua.compareRefs(mFuncRef, rhs->mFuncRef);
	}

	bool LuaEventListener::compareListeners(const IEventListener *rhs) const
	{
		const LuaEventListener *rhsLua = dynamic_cast<const LuaEventListener *>(rhs);
		if (rhsLua)
		{
			return compareListeners(rhsLua);
		}
		return false;
	}
}
}
