#pragma once

#include <string>
#include <map>
#include <vector>
using namespace std;

#include <base/handle.h>
#include <base/imanaged.h>
using namespace am::base;

#include <ui/ievent_listener.h>
#include <ui/ui_inspector.h>
#include <ui/event_interface.h>
using namespace am::ui;

#include <util/idata.h>
using namespace am::util;

#include "game_object.h"
#include "camera.h"
#include "map.h"
#include "character.h"
#include "quest.h"
#include "loading_state.h"
#include "iattribute_data.h"

#include <log/logger.h>

namespace am {

namespace gfx {
	class Layer;
}
using namespace am::gfx;

namespace util {
namespace data {
	class Table;
}
}

namespace game {

	class Engine;

	class Game : public IEventListener, public EventInterface, public IAttributeData {
	public:
		Game(Engine *engine = NULL);
		~Game();

		void deinit();

		bool addMap(Map *map);
		bool removeMap(Map *map);
		bool removeMap(const char *mapName);
		void removeAllMaps();
		bool hasMap(const char *mapName);

		Map *getMapLua(const char *mapName);
		Map *getMapLua(const string &mapName);
		Map *getCurrentMap();
		void setCurrentMap(Map *map, bool addMap = true);
		void setCurrentMap(const char *mapName);
		void setCurrentMap(const string &mapName);

		Layer *getGameLayer();
		Layer *getBackground();
		Layer *getItemLayer();
		Layer *getCharacterLayer();
		Layer *getForeground();

		bool addGameObjectToMap(GameObject *object);
		bool removeGameObjectFromMap(GameObject *object);
		bool hasGameObjectInMap(GameObject *object) const;

		void moveObjectToMap(GameObject *object, const char *mapName, float x, float y, bool setAsCurrent = true);
		void moveObjectToMap(GameObject *object, Map *map, float x, float y, bool setAsCurrent = true);
		void moveObjectToMapGrid(GameObject *object, const char *mapName, int x, int y, bool setAsCurrent = true);
		void moveObjectToMapGrid(GameObject *object, Map *map, int x, int y, bool setAsCurrent = true);

		void startGame();
		bool hasStarted() const;

		void saveGame(const char *saveName);
		int loadGame(const char *saveName);

		virtual void onEvent(MouseEvent *e);

		void setMainCharacter(Character *character);
		Character *getMainCharacter();

		string getLoadingFile() const;

		void addCharDefinition(Character *character, const char *name);
		Character *getCharDefinition(const char *name);
		void addItemDefinition(Item *item, const char *name);
		Item *getItemDefinition(const char *name);

		Camera *getCamera();

		void update(float dt);

		// GameObject
		GameObject *getGameObject(const char *id) const;
		bool registerGameObject(GameObject *obj);
		void deregisterGameObject(const char *id);
		void deregisterGameObject(GameObject *obj);

		// Dialogue
		bool addDialogue(Dialogue *dialogue);
		bool removeDialogue(const char *id);
		void removeAllDialogue();
		Dialogue *getDialogue(const char *id);
		void getAvailableDialogues(vector<Dialogue *> &result, const GameObject *talker, const GameObject *talkedTo);

		// Quest
		bool addQuest(Quest *quest);
		bool removeQuest(const char *questId);
		Quest *getQuest(const char *questId);

		// LoadingState
		LoadingState *getLoadingState();

		void setScenarioName(const char *scenarioName);
		const char *getScenarioName() const;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:
		
		Handle<Map> mCurrentMap;

		Engine *mEngine;

		Camera mCamera;
		bool mStarted;
		// Comes from the current screen.
		ObjectList *mActiveObjects;

		typedef map<string, Handle<Map> > MapMap;
		MapMap mMaps;

		typedef vector<string> LoadingFilesStack;
		LoadingFilesStack mLoadingFiles;

		typedef map<string, bool> FilesLoadedMap;
		FilesLoadedMap mFilesLoaded;

		typedef map<string, Handle<Item> >ItemMap;
		ItemMap mItemDefinitions;

		typedef map<string, Handle<Character> > CharacterMap;
		CharacterMap mCharDefinitions;

		string mScenarioName;

		Handle<Layer> mGameLayer;
		Handle<Layer> mBackground;
		Handle<Layer> mItemLayer;
		Handle<Layer> mCharacterLayer;
		Handle<Layer> mForeground;

		Handle<Character> mMainCharacter;

		Handle<LoadingState> mLoadingState;

		typedef map<string, GameObject *> GameObjectIdMap;
		GameObjectIdMap mGameObjects;

		typedef map<string, Dialogue *> DialogueMap;
		DialogueMap mDialogueMap;

		typedef map<string, Quest *> QuestMap;
		QuestMap mQuestMap;

		template <class T>
		void addDefinition(T *def, map< string, Handle<T> > &defMap, const char *name)
		{
			if (def == NULL || name == NULL || name[0] == '\0')
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
			if (name == NULL || name[0] == '\0')
			{
				return NULL;
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
			if (index > 0)
			{
				filename = "data/defs/";
				mLoadingFiles.push_back(str.substr(0, index));
				filename += mLoadingFiles.back();
				filename += ".lua";
				charname = str.substr(index + 1);
			}
			else if (!mLoadingFiles.empty())
			{
				filename = "data/defs/";
				filename += mLoadingFiles.back();
				filename += ".lua";
				charname = str;
			}
			else
			{
				filename = "data/defs/default.lua";
				charname = str;
			}

			// Check if the file has already been loaded and previously did
			// not find the character.
			auto findFile = mFilesLoaded.find(filename);
			if (findFile != mFilesLoaded.end() && findFile->second)
			{
				return NULL;
			}

			mFilesLoaded[filename] = true;

			if (!mEngine->getLua().loadFile(filename.c_str()))
			{
				stringstream ss;
				ss << "Error loading file: '" << filename << "':\n";
				mEngine->getLua().printStack(ss);
				am_log("LUAERR", ss);
				return NULL;
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
			return NULL;
		}

		data::IData *saveGameData();
		void loadGameData(data::Table *obj);
	};

}
}
