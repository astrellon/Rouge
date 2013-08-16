#pragma once

#include <string>
#include <map>
#include <vector>
using namespace std;

#include <game/engine.h>
using namespace am::game;

#include <lua/lua_state.h>
using namespace am::lua;

#include <base/handle.h>
#include <base/imanaged.h>
using namespace am::base;

namespace am {
namespace util {

	class IDefinitionManager {
	
	protected:

		typedef vector<string> LoadingFilesStack;
		LoadingFilesStack mLoadingFiles;

		typedef map<string, bool> FilesLoadedMap;
		FilesLoadedMap mFilesLoaded;

		virtual const char *getBaseDefinitionPath() const = 0;
		
		template <class T>
		void addDefinition(T *def, map< string, Handle<T> > &defMap, const char *name)
		{
			if (def == nullptr || name == nullptr || name[0] == '\0')
			{
				return;
			}
			if (!mLoadingFiles.empty())
			{
				string path = mLoadingFiles.back();
				path += ':';
				path += name;
				defMap[path] = def;
				return;
			}
			defMap[string(name)] = def;
		}
		template <class T>
		T *getDefinition(map< string, Handle<T> > &defMap, const char *name)
		{
			if (name == nullptr || name[0] == '\0')
			{
				return nullptr;
			}
			string str(name);
			size_t index = str.find(':');
			if (index == string::npos && !mLoadingFiles.empty())
			{
				string temp = mLoadingFiles.back();
				temp += ':';
				str = temp + str;
			}
			auto find = defMap.find(str);
			if (find != defMap.end())
			{
				return find->second;
			}
			string filename;
			string charname;
			const char *base = getBaseDefinitionPath();
			if (index > 0)
			{
				filename = base;
				mLoadingFiles.push_back(str.substr(0, index));
				filename += mLoadingFiles.back();
				filename += ".lua";
				charname = str.substr(index + 1);
			}
			else if (!mLoadingFiles.empty())
			{
				filename = base;
				filename += mLoadingFiles.back();
				filename += ".lua";
				charname = str;
			}
			else
			{
				filename = base;
				filename += "default.lua";
				charname = str;
			}

			// Check if the file has already been loaded and previously did
			// not find the character.
			auto findFile = mFilesLoaded.find(filename);
			if (findFile != mFilesLoaded.end() && findFile->second)
			{
				return nullptr;
			}

			mFilesLoaded[filename] = true;
			LuaState &lua = Engine::getEngine()->getLua();

			//if (!mEngine->getLua().loadFile(filename.c_str()))
			if (!lua.loadFile(filename.c_str()))
			{
				stringstream ss;
				ss << "Error loading file: '" << filename << "':\n";
				lua.printStack(ss);
				am_log("LUAERR", ss);
				return nullptr;
			}

			if (!mLoadingFiles.empty())
			{
				mLoadingFiles.pop_back();
			}

			find = defMap.find(str);
			if (find != defMap.end())
			{
				return find->second;
			}
			return nullptr;
		}

	};

}
}
