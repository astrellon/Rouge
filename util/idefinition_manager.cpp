#include "idefinition_manager.h"

#include <game/engine.h>
using namespace am::game;

namespace am {
namespace util {

	LuaState &IDefinitionManager::getLuaDefinition() const
	{
		return Engine::getEngine()->getLua();
	}
		
}
}
