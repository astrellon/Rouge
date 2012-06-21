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
#include "screen.h"

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

		Screen *getScreen(const char *screenName);
		Screen *getScreen(const string &screenName);
		Screen *getCurrentScreen();
		void setCurrentScreen(Screen *screen);
		void setCurrentScreen(const char *screenName);
		void setCurrentScreen(const string &screenName);

		Layer *getGameLayer();
		Layer *getBackground();
		Layer *getItemLayer();
		Layer *getCharacterLayer();
		Layer *getForeground();

		void addGameObject(GameObject *object);
		void removeGameObject(GameObject *object);
		bool hasGameObject(GameObject *object) const;

		Camera *getCamera();

		void update(float dt);

	protected:
		
		Handle<Screen> mCurrentScreen;

		Engine *mEngine;

		Camera mCamera;
		// Comes from the current screen.
		ObjectList *mActiveObjects;

		typedef map<string, Handle<Screen> > ScreenMap;
		ScreenMap mScreens;
		
		Handle<Layer> mGameLayer;
		Handle<Layer> mBackground;
		Handle<Layer> mItemLayer;
		Handle<Layer> mCharacterLayer;
		Handle<Layer> mForeground;

		ObjectList::iterator findGameObject(GameObject *object) const;
	};

}
}
