#pragma once

#include <string>
#include <map>
#include <vector>
using namespace std;

#include "handle.h"
#include "imanaged.h"
using namespace am::util;

#include "game_object.h"
#include "camera.h"
//#include "screen.h"

#include "map.h"

namespace am {

namespace gfx {
	class Layer;
}
using namespace am::gfx;

namespace base {

	class Engine;

	class Game : public IManaged {
	public:
		Game(Engine *engine);
		~Game();

		/*Screen *getScreen(const char *screenName);
		Screen *getScreen(const string &screenName);
		Screen *getCurrentScreen();
		void setCurrentScreen(Screen *screen);
		void setCurrentScreen(const char *screenName);
		void setCurrentScreen(const string &screenName);*/
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

		//void moveObjectToScreen(GameObject *object, const char *screenName, float x, float y, bool setAsCurrent = true);
		//void moveObjectToScreen(GameObject *object, Screen *screen, float x, float y, bool setAsCurrent = true);
		void moveObjectToMap(GameObject *object, const char *mapName, float x, float y, bool setAsCurrent = true);
		void moveObjectToMap(GameObject *object, Map *map, float x, float y, bool setAsCurrent = true);

		Camera *getCamera();

		void update(float dt);

	protected:
		
		//Handle<Screen> mCurrentScreen;
		Handle<Map> mCurrentMap;

		Engine *mEngine;

		Camera mCamera;
		// Comes from the current screen.
		ObjectList *mActiveObjects;

		//typedef map<string, Handle<Screen> > ScreenMap;
		//ScreenMap mScreens;
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
