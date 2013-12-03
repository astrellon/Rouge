#include "idefinition_manager.h"

#include <game/engine.h>

namespace am {
namespace util {

	lua::LuaState &IDefinitionManager::getLuaDefinition() const
	{
		return Engine::getEngine()->getLua();
	}

	bool IDefinitionManager::loadDefinitionFile(const char *path, const char *filename)
	{
		mLoadingFiles.push_back(std::string(filename));
		return _loadDefinitionFile(path);
	}
	
	bool IDefinitionManager::_loadDefinitionFile(const char *filename)
	{
		// Check if the file has already been loaded and previously did
		// not find the character.
		auto findFile = mFilesLoaded.find(filename);
		if (findFile != mFilesLoaded.end() && findFile->second)
		{
			return true;
		}

		mFilesLoaded[filename] = true;
		lua::LuaState &lua = getLuaDefinition();

		if (!lua.loadFile(filename))
		{
			std::stringstream ss;
			ss << "Error loading file: '" << filename << "':\n";
			lua.printStack(ss);
			am_log("LUAERR", ss);
			return false;
		}

		if (!mLoadingFiles.empty())
		{
			mLoadingFiles.pop_back();
		}
		return true;
	}
}
}
