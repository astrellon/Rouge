#pragma once

#include <string>
#include <map>
#include <vector>

#include <lua/lua_state.h>

#include <log/logger.h>

#include <base/handle.h>
#include <base/imanaged.h>

namespace am {
namespace util {

	class IDefinitionManager 
	{
	public:

		IDefinitionManager();

		bool loadDefinitionFile(const char *path, const char *filename);
		
	protected:

		void setReloadDefinitionFiles(bool reload);
		bool getReloadDefinitionFiles() const;

		typedef std::vector<std::string> LoadingFilesStack;
		LoadingFilesStack mLoadingFiles;

		typedef std::map<std::string, bool> FilesLoadedMap;
		FilesLoadedMap mFilesLoaded;

		virtual const char *getBaseDefinitionPath(int id) const = 0;

		virtual lua::LuaState &getLuaDefinition() const;

		bool _loadDefinitionFile(const char *filename);
		
		template <class T>
		void addDefinition(T *def, std::map< std::string, base::Handle<T> > &defMap, const char *name)
		{
			if (def == nullptr || name == nullptr || name[0] == '\0')
			{
				return;
			}
			if (!mLoadingFiles.empty())
			{
				std::string path = mLoadingFiles.back();
				path += ':';
				path += name;
				def->setLoadedName(path.c_str());
				defMap[path] = def;
				return;
			}
			defMap[std::string(name)] = def;
		}

		template <class T>
		T *findDefinition(std::map< std::string, base::Handle<T> > &defMap, const char *name)
		{
			if (name == nullptr || name[0] == '\0')
			{
				return nullptr;
			}
			std::string str(name);
			size_t index = str.find(':');
			if (index == std::string::npos && !mLoadingFiles.empty())
			{
				std::string temp = mLoadingFiles.back();
				temp += ':';
				str = temp + str;
			}

			auto find = defMap.find(str);
			if (find != defMap.end())
			{
				return find->second;
			}
			return nullptr;
		}

		/**
		 *
		 */
		template <class T>
		T *getDefinition(std::map< std::string, base::Handle<T> > &defMap, const char *name, bool reload, int id)
		{
			if (name == nullptr || name[0] == '\0')
			{
				return nullptr;
			}
			std::string str(name);
			size_t index = str.find(':');
			if (index == std::string::npos && !mLoadingFiles.empty())
			{
				std::string temp = mLoadingFiles.back();
				temp += ':';
				str = temp + str;
			}

			if (!reload)
			{
				auto find = defMap.find(str);
				if (find != defMap.end())
				{
					return find->second;
				}
			}
			std::string filename;
			std::string charname;
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

			if (reload)
			{
				setReloadDefinitionFiles(true);
			}

			if (!_loadDefinitionFile(filename.c_str()))
			{
				return nullptr;
			}

			if (reload)
			{
				setReloadDefinitionFiles(false);
			}

			auto find = defMap.find(str);
			if (find != defMap.end())
			{
				return find->second;
			}
			return nullptr;
		}

	private:

		bool mReloadDefinitionFiles;

	};

}
}
