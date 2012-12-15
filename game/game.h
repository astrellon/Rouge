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
using namespace am::ui;

#include "game_object.h"
#include "camera.h"
#include "map.h"
#include "character.h"
#include "quest.h"

namespace am {

namespace gfx {
	class Layer;
}
using namespace am::gfx;

namespace game {

	class Engine;

	class Game : public IEventListener {
	public:
		Game(Engine *engine = NULL);
		~Game();

		void deinit();

		bool addMap(Map *map);
		bool removeMap(Map *map);
		bool removeMap(const char *mapName);
		void removeAllMaps();

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

		bool addGameObject(GameObject *object);
		bool removeGameObject(GameObject *object);
		bool hasGameObject(GameObject *object) const;

		void moveObjectToMap(GameObject *object, const char *mapName, float x, float y, bool setAsCurrent = true);
		void moveObjectToMap(GameObject *object, Map *map, float x, float y, bool setAsCurrent = true);
		void moveObjectToMapGrid(GameObject *object, const char *mapName, int x, int y, bool setAsCurrent = true);
		void moveObjectToMapGrid(GameObject *object, Map *map, int x, int y, bool setAsCurrent = true);

		virtual void onEvent(MouseEvent *e);

		void setMainCharacter(Character *character);
		Character *getMainCharacter();

		Camera *getCamera();

		void update(float dt);

		// GameObject
		GameObject *getByGameId(const char *id) const;
		void registerGameObject(GameObject *obj);
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

	protected:
		
		Handle<Map> mCurrentMap;

		Engine *mEngine;

		Camera mCamera;
		// Comes from the current screen.
		ObjectList *mActiveObjects;

		typedef map<string, Handle<Map> > MapMap;
		MapMap mMaps;

		Handle<Layer> mGameLayer;
		Handle<Layer> mBackground;
		Handle<Layer> mItemLayer;
		Handle<Layer> mCharacterLayer;
		Handle<Layer> mForeground;

		Handle<Character> mMainCharacter;

		typedef map<string, GameObject *> GameObjectIdMap;
		GameObjectIdMap mGameObjects;

		typedef map<string, Dialogue *> DialogueMap;
		DialogueMap mDialogueMap;

		typedef map<string, Quest *> QuestMap;
		QuestMap mQuestMap;

	};

}
}
