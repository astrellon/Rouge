#pragma once

#include <string>
#include <map>
#include <vector>
using namespace std;

#include <lua/lua_state.h>
using namespace am::lua;

#include <log/logger.h>

#include <base/handle.h>
#include <base/imanaged.h>
using namespace am::base;

namespace am {
namespace util {

	class IDefinitionManager {
	public:

		bool loadDefinitionFile(const char *path, const char *filename);
		
	protected:

		typedef vector<string> LoadingFilesStack;
		LoadingFilesStack mLoadingFiles;

		typedef map<string, bool> FilesLoadedMap;
		FilesLoadedMap mFilesLoaded;

		virtual const char *getBaseDefinitionPath(int id) const = 0;

		virtual LuaState &getLuaDefinition() const;

		bool _loadDefinitionFile(const char *filename);
		
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
				def->setLoadedName(path.c_str());
				defMap[path] = def;
				return;
			}
			defMap[string(name)] = def;
		}

		template <class T>
		T *getDefinition(map< string, Handle<T> > &defMap, const char *name, int id = 0)
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
			const char *base = getBaseDefinitionPath(id);
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

			if (!_loadDefinitionFile(filename.c_str()))
			{
				return nullptr;
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
