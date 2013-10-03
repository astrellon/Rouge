#include "race.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int Race::LUA_ID = LUA_ID_RACE;
	const char *Race::LUA_TABLENAME = LUA_TABLE_RACE;

	Race::Race(const char *raceName) :
		mRaceName(raceName)
	{

	}
	Race::~Race()
	{

	}

	const char *Race::getRaceName() const
	{
		return mRaceName.c_str();
	}

}
}
