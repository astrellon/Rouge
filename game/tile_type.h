#pragma once

#include <base/imanaged.h>
using namespace am::base;

#include <map>
#include <string>
using namespace std;

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace game {

	class TileType : public IManaged {
	public:
		TileType(const char *name, const char *fullName = nullptr);
		~TileType();

		const char *getName() const;

		void setFullName(const char *name);
		const char *getFullName() const;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;
		
	protected:

		string mName;
		string mFullName;

	};

}
}
