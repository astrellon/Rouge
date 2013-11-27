#pragma once

#include <base/imanaged.h>

#include <map>
#include <string>
using namespace std;

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace game {

	class TileType : public am::base::IManaged {
	public:
		TileType(const char *name, const char *fullName = nullptr);
		~TileType();

		virtual const char *getName() const;

		virtual void setFullName(const char *name);
		virtual const char *getFullName() const;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;
		
	protected:

		string mName;
		string mFullName;

	};

}
}
