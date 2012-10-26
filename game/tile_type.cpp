#include "tile_type.h"

#include <log/logger.h>

#include <util/utils.h>

#include <sstream>
using namespace std;

namespace am {
namespace game {

	TileType::TileTypeMap TileType::sTileTypes;

	TileType::TileType(const char *name, const char *fullName) :
		mName(name)
	{
		if (fullName)
		{
			mFullName = fullName;
		}
	}
	TileType::~TileType()
	{
	}

	const char *TileType::getName() const
	{
		return mName.c_str();
	}

	void TileType::setFullName(const char *name)
	{
		mFullName = name;
	}
	const char *TileType::getFullName() const
	{
		return mFullName.c_str();
	}

	bool TileType::loadFromDef(JsonValue value)
	{
		if (value.has("fullName", JV_STR))
		{
			mFullName = value["fullName"].getCStr();
		}
		return true;
	}
	bool TileType::loadFromDef(LuaState &lua)
	{
		if (!lua_istable(lua, -1))
		{
			return false;
		}
		if (lua.isTableString("fullName"))
		{
			mFullName = lua_tostring(lua, -1);
			lua.pop(1);
		}
		return true;
	}

	void TileType::addTileType(TileType *type)
	{
		if (type == NULL)
		{
			return;
		}
		sTileTypes[type->mName] = type;
	}
	TileType *TileType::getTileType(const char *name)
	{
		return getTileType(string(name));
	}
	TileType *TileType::getTileType(const string &name)
	{
		TileTypeMap::iterator iter = sTileTypes.find(name);
		if (iter == sTileTypes.end())
		{
			return NULL;
		}
		return iter->second;
	}

	bool TileType::loadStandardTileTypes(const char *filename)
	{
		JsonValue loaded = JsonValue::import_from_file(filename);
		if (loaded.getType() == JV_INT)
		{
			stringstream errss;
			errss << "Unable to load standard tile types: " << loaded.getInt();
			am_log("TILE", errss);
			return false;
		}

		if (loaded.getType() != JV_OBJ)
		{
			stringstream errss;
			errss << "Unable to load standard tile types, loaded file was of type: " << loaded.getTypeName();
			am_log("TILE", errss);
			return false;
		}

		JsonObject *obj = loaded.getObj();
		JsonObject::iterator iter;
		for (iter = obj->begin(); iter != obj->end(); ++iter)
		{
			if (iter->second.getType() == JV_CMT)
			{
				continue;
			}
			if (iter->second.getType() != JV_OBJ)
			{
				stringstream errss;
				errss << "Tile type '" << iter->first << "' was of type '" << iter->second.getTypeName() << "' and not an object.";
				am_log("TILE", errss);
				continue;
			}

			string tileName = Utils::toLowerCase(iter->first.c_str());

			TileType *loadedType = new TileType(tileName.c_str());
			if (!loadedType->loadFromDef(iter->second))
			{
				stringstream errss;
				errss << "Failed to load '" << tileName << "' object in definition was invalid.";
				am_log("TILE", errss);
				delete loadedType;
				continue;
			}

			stringstream ss;
			ss << "Added tile type '" << loadedType->getName() << "'";
			am_log("TILE", ss);
			addTileType(loadedType);
		}
		return true;
	}
	bool TileType::loadStandardTileTypesLua(const char *filename)
	{
		LuaState lua;
		if (!lua.loadFile(filename))
		{
			lua.logStack("TITLE");
			lua.close();
			stringstream errss;
			errss << "Unable to load standard tile types";
			am_log("TILE", errss);
			return false;
		}

		lua_getglobal(lua, "types");
		if (!lua_istable(lua, -1))
		{
			stringstream errss;
			errss << "Unable to load standard tile types, loaded file was of type: " << lua_typename(lua, -1);
			am_log("TILE", errss);
			lua.close();
			return false;
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
					errss << "Tile type '" << lua_tostring(lua, -2) << "' was of type '" << lua_typename(lua, -1) << "' and not an object.";
					am_log("TILE", errss);
					continue;
				}

				string tileName = Utils::toLowerCase(lua_tostring(lua, -2));

				TileType *loadedType = new TileType(tileName.c_str());
				if (!loadedType->loadFromDef(lua))
				{
					stringstream errss;
					errss << "Failed to load '" << tileName << "' object in definition was invalid.";
					am_log("TILE", errss);
					delete loadedType;
					continue;
				}

				stringstream ss;
				ss << "Added tile type '" << loadedType->getName() << "'";
				am_log("TILE", ss);
				addTileType(loadedType);
			}
			/* removes 'value'; keeps 'key' for next iteration */
			lua_pop(lua, 1);
		}
	}
	
}
}
