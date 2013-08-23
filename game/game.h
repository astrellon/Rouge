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
#include <util/idefinition_manager.h>
using namespace am::util;

#include "game_object.h"
#include "camera.h"
#include "map.h"
#include "quest.h"
#include "character.h"
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

	class Game : public IDefinitionManager, public IEventListener, public EventInterface, public IAttributeData {
	public:
		Game(Engine *engine = nullptr);
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
		void onGameTick();
		void setCurrentGameTickLength(float dt);
		//void nextObjectTurn();

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

		void setGenericDeadGraphic(Sprite *graphic);
		Sprite *getGenericDeadGraphic() const;

		// This is different to pausing the game.
		// As it relates to the game tick itself waiting for
		// something to happen like user input.
		void setGameTickPaused(bool paused);
		bool isGameTickPaused() const;

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

		typedef map<string, Handle<Item> >ItemMap;
		ItemMap mItemDefinitions;

		typedef map<string, Handle<Character> > CharacterMap;
		CharacterMap mCharDefinitions;

		virtual const char *getBaseDefinitionPath(int id) const;

		string mScenarioName;

		Handle<Sprite> mGenericDeadGraphic;

		Handle<Layer> mGameLayer;
		Handle<Layer> mBackground;
		Handle<Layer> mItemLayer;
		Handle<Layer> mCharacterLayer;
		Handle<Layer> mForeground;

		Handle<Character> mMainCharacter;

		Handle<LoadingState> mLoadingState;

		int mGameTickPosition;
		bool mGameTickPaused;
		float mCurrentTickDt;

		typedef map<string, Handle<GameObject> > GameObjectIdMap;
		GameObjectIdMap mGameObjects;

		typedef map<string, Dialogue *> DialogueMap;
		DialogueMap mDialogueMap;

		typedef map<string, Handle<Quest> > QuestMap;
		QuestMap mQuestMap;

		void endGameTick();

		data::IData *saveGameData();
		void loadGameData(data::Table *obj);

		void setTickPositionMainChar();
	};

}
}
