#include "tile_type.h"

#include <log/logger.h>

#include <util/utils.h>
using namespace am::util;

#include <sstream>

#include "engine.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int TileType::LUA_ID = LUA_ID_TILETYPE;
	const char *TileType::LUA_TABLENAME = LUA_TABLE_TILETYPE;

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

}
}
