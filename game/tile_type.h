#pragma once

#include <base/imanaged.h>

#include <map>
#include <string>

#include <lua/lua_state.h>

namespace am {
namespace game {

	class TileType : public base::IManaged 
	{
	public:
		TileType(const char *name, const char *fullName = nullptr);
		~TileType();

		virtual const char *getName() const;

		virtual void setFullName(const char *name);
		virtual const char *getFullName() const;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;
		
	protected:

		std::string mName;
		std::string mFullName;

	};

}
}
