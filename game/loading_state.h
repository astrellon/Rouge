#pragma once

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <string>
#include <map>
#include <vector>
using namespace std;

#include "game_object.h"
#include "dialogue_component.h"

namespace am {
namespace game {

	class Game;

	// Class for storing information about the loading process.
	// This involves storing which maps to load, which maps each 
	// character needs to be loaded into and the starting dialogue
	// for each dialogue component.
	// 
	// The need for this comes from the fact that the maps store the
	// information about a game object. So we need to load all the 
	// game objects before loading the maps so that the maps know
	// to only apply the information that is not stored with a game
	// object such as event listeners.

	class LoadingState : public IManaged {
	public:

		typedef struct _GameObjectInfo {
			Handle<GameObject> gameObj;
			bool hasDestination;

			_GameObjectInfo();
			_GameObjectInfo(GameObject *gameObj);
		} GameObjectInfo;

		typedef map<string, bool> MapsToLoad;
		typedef map<string, GameObjectInfo> GameObjectInfoMap;
		typedef map<string, vector<Handle<GameObject> > > GameObjectsToMap;
		typedef map<string, vector<Handle<DialogueComponent> > > StartDialogueMap;

		LoadingState();
		~LoadingState();

		void addMapToLoad(const char *map);
		const MapsToLoad &getMapsToLoad() const;

		void setCurrentMap(const char *mapName);
		const char *getCurrentMap() const;

		void setMainCharacter(const char *mainChar);
		const char *getMainCharacter() const;

		// For keeping track of which map each game object needs to be added
		// to once the maps are loaded.
		void addGameObjectToMap(GameObject *gameObj, const char *mapName);
		const GameObjectsToMap &getGameObjectsToMap() const;

		// For keeping track of which gameId each game object wants to have.
		void setGameId(const char *gameId, GameObject *gameObj);
		GameObject *getGameId(const char *gameId) const;

		void setGameObjHasDestination(const char *gameId, bool hasDestination);
		bool getGameObjHasDestination(const char *gameId) const;

		const GameObjectInfoMap &getGameInfoMap() const;
		const GameObjectInfo &getGameInfo(const char *gameId) const;

		// For keeping track of the starting dialogue for a dialogue component.
		void setStartDialogue(const char *startDialogue, DialogueComponent *comp);
		const StartDialogueMap &getStartDialogueMap() const;

		void postLoad(Game *game);

	protected:

		MapsToLoad mMapsToLoad;
		GameObjectInfoMap mGameObjectInfoMap;
		GameObjectsToMap mGameObjectsToMap;
		StartDialogueMap mStartDialogueMap;

		string mCurrentMap;
		string mMainCharacter;
	};

}
}
