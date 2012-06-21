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
