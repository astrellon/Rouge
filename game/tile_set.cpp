#include "tile_set.h"

#include <log/logger.h>
#include <sstream>

namespace am {
namespace game {

	TileSet::TileSet()
	{
	}
	TileSet::TileSet(const char *name) :
		mName(name)
	{
	}
	TileSet::~TileSet()
	{
	}

	void TileSet::setName(const char *name)
	{
		mName = name;
	}
	string TileSet::getName() const
	{
		return mName;
	}

	void TileSet::setFullName(const char *name)
	{
		mFullName = name;
	}
	string TileSet::getFullName() const
	{
		return mFullName;
	}

	void TileSet::addTile(Tile *tile)
	{
		if (tile)
		{
			mTiles[tile->getName()] = tile;
		}
	}
	void TileSet::removeTile(Tile *tile)
	{
		if (tile)
		{
			removeTile(tile->getName().c_str());
		}
	}
	void TileSet::removeTile(const char *tileName)
	{
		if (tileName)
		{
			TileMap::iterator iter = mTiles.find(string(tileName));
			if (iter != mTiles.end())
			{
				mTiles.erase(iter);
			}
		}
	}
	bool TileSet::hasTile(Tile *tile)
	{
		if (tile)
		{
			return mTiles.find(string(tile->getName())) != mTiles.end();
		}
		return false;
	}
	bool TileSet::hasTile(const char *tileName)
	{
		if (tileName)
		{
			return mTiles.find(string(tileName)) != mTiles.end();
		}
		return false;
	}
	Tile *TileSet::getTile(const char *tileName)
	{
		if (tileName)
		{
			TileMap::iterator iter = mTiles.find(string(tileName));
			if (iter == mTiles.end())
			{
				return NULL;
			}
			return iter->second.get();
		}
		return NULL;
	}

	void TileSet::loadDef(JsonValue value)
	{
		if (value.has("fullName", JV_STR))
		{
			mFullName = value["fullName"].getCStr();
		}
		if (value.has("tiles", JV_OBJ))
		{
			JsonObject *tiles = value["tiles"].getObj();
			JsonObject::iterator iter;
			for (iter = tiles->begin(); iter != tiles->end(); ++iter)
			{
				JsonValue tileDef = iter->second;
				if (tileDef.getType() == JV_OBJ)
				{
					Tile *tile = new Tile(iter->first.c_str());
					tile->loadDef(tileDef);
					addTile(tile);
				}
			}
		}
		else
		{
			stringstream ss;
			ss << "Tile set '" << mName << "' definition does not have a set of tiles";
			am_log("SET", ss);
		}
	}
	void TileSet::loadDef(LuaState &lua)
	{
		if (!lua_istable(lua, -1))
		{
			return;
		}
		if (lua.isTableString("fullName"))
		{
			mFullName = lua_tostring(lua, -1);
			lua.pop(1);
		}
		lua_getglobal(lua, "tiles");
		if (!lua_istable(lua, -1))
		{
			stringstream ss;
			ss << "Tile set '" << mName << "' definition does not have a set of tiles";
			am_log("SET", ss);
			lua.pop(1);
			return;
		}
		/* table is in the stack at index 't' */
		lua_pushnil(lua);  /* first key */
		while (lua_next(lua, -2) != 0) 
		{
			/* uses 'key' (at index -2) and 'value' (at index -1) */
			if (lua_isstring(lua, -2))
			{
				if (!lua_istable(lua, -1))
				{
					stringstream errss;
					errss << "Tile def '" << lua_tostring(lua, -2) << "' was of type '" << lua_typename(lua, -1) << "' and not an object.";
					am_log("TILE", errss);
					continue;
				}
				Tile *tile = new Tile(lua_tostring(lua, -2));
				tile->loadDef(lua);
				addTile(tile);
			}
			/* removes 'value'; keeps 'key' for next iteration */
			lua_pop(lua, 1);
		}
	}

}
}
