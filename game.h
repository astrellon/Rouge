#pragma once

#include <string>
#include <map>
using namespace std;

#include "handle.h"
using namespace am::util;

namespace am {

namespace gfx {
	class GfxEngine;
	class Layer;
}
using namespace am::gfx;

namespace base {

	class Screen;
	class Engine;

	class Game {
	public:
		Game(Engine *engine, GfxEngine *gfxEngine);
		~Game();

		//void setMap(Map *map);
		//Map *getMap();

		//int loadMap(const string &filename);
		//string getMapFilename() const;

		Screen *getCurrentScreen();
		void addScreen(Screen *screen);
		void setCurrentScreen(Screen *screen);
		void setCurrentScreen(const char *screenName);
		void setCurrentScreen(const string &screenName);

		Layer *getGameLayer();
		Layer *getBackground();
		Layer *getItemLayer();
		Layer *getCharacterLayer();
		Layer *getForeground();

	protected:
		//Map *mMap;
		Screen *mCurrentScreen;

		typedef map<string, Screen *> ScreenMap;
		ScreenMap mScreens;

		Engine *mEngine;
		GfxEngine *mGfxEngine;

		Handle<Layer> mGameLayer;
		Handle<Layer> mBackground;
		Handle<Layer> mItemLayer;
		Handle<Layer> mCharacterLayer;
		Handle<Layer> mForeground;
	};

}
}
