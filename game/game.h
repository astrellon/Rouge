#pragma once

#include <string>
#include <map>
#include <vector>
using namespace std;

#include <base/handle.h>
#include <base/imanaged.h>
using namespace am::base;

#include "game_object.h"
#include "camera.h"
#include "map.h"

namespace am {

namespace gfx {
	class Layer;
}
using namespace am::gfx;

namespace game {

	class Engine;

	class Game : public IManaged {
	public:
		Game(Engine *engine);
		~Game();

		Map *getMap(const char *mapName);
		Map *getMap(const string &mapName);
		Map *getCurrentMap();
		void setCurrentMap(Map *map);
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

		Camera *getCamera();

		void update(float dt);

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
	};

}
}
