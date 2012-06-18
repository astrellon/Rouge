#include "engine.h"

#include "game.h"
#include "tile.h"
#include "json_value.h"
using namespace am::util;

#include <sstream>

#include "logger.h"

namespace am {
namespace base {

	Engine::Engine() :
		mCurrentGame(NULL)
	{
	}
	Engine::~Engine() 
	{
		//deregisterTiles();
	}

	Screen *Engine::getScreen(const char *screenName)
	{
		return getScreen(string(screenName));
	}
	Screen *Engine::getScreen(const string &screenName)
	{
		ScreenMap::iterator iter = mScreens.find(screenName);
		if (iter != mScreens.end())
		{
			return iter->second.get();
		}

		stringstream ss;
		ss << "data/screens/" << screenName << "/screen.ssff";
		JsonValue loaded = JsonValue::import_from_file(ss.str().c_str());
		if (loaded.getType() != JV_OBJ)
		{
			stringstream errss;
			errss << "Unable to load screen '" << screenName << "', using the path '";
			errss << ss.str() << '\''; 
			am_log("SCREEN", errss.str().c_str());
			return NULL;
		}

		Handle<Screen> screen(new Screen(screenName.c_str()));
		screen->loadDef(loaded);

		mScreens[screenName] = screen;

		return screen.get();
	}
	
	void Engine::init()
	{

	}
	void Engine::deinit()
	{

	}
	void Engine::update(float dt)
	{
		if (mCurrentGame.get())
		{
			mCurrentGame->update(dt);
		}
	}
	void Engine::mouseFunc(int mouseButton, int x, int y)
	{
		
	}
	void Engine::keyboardFunc(const bool *keys, int key)
	{

	}

	void Engine::setCurrentGame(Game *game)
	{
		mCurrentGame = game;
	}
	Game *Engine::getCurrentGame()
	{
		return mCurrentGame.get();
	}

}
}
