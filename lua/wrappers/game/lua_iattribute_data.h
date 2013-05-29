#pragma once

extern "C" 
{
#	include <lua/src/lstate.h>
}

namespace am {
namespace game {
	class IAttributeData;
}

namespace lua {
namespace game {

	int IAttributeData_attrs(lua_State *lua, am::game::IAttributeData *attr);

}
}
}
