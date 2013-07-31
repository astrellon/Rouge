#include "game_system.h"

#include <game/engine.h>
#include <game/pathfinder.h>

#include <gfx/gfx_sprite.h>
#include <gfx/gfx_text_field.h>
#include <gfx/gfx_layer.h>
#include <gfx/gfx_font.h>
#include <gfx/gfx_asset.h>
#include <gfx/scale_nine.h>
#include <gfx/gfx_text_list.h>
#include <gfx/gfx_log_listener.h>
#include <gfx/gfx_engine.h>

#include <sfx/sfx_engine.h>
using namespace am::sfx;

#include <log/logger.h>

#include <ui/mouse_manager.h>
#include <ui/keyboard_manager.h>

#include <math/transform.h>

#include <math/transform.h>
#include <math/vector.h>

#include <sstream>

#include <base/handle.h>

#include <util/colour.h>

using namespace std;
using namespace am::math;

namespace am {
namespace sys {

	GameSystem *GameSystem::sGameSystem = nullptr;

	GameSystem *GameSystem::createGameSystem(ISystem *linked, Engine *engine)
	{
		sGameSystem = new GameSystem(linked, engine);
		return sGameSystem;
	}
	GameSystem *GameSystem::getGameSystem()
	{
		return sGameSystem;
	}

	GameSystem::GameSystem(ISystem *linked, Engine *engine) :
		mLinkedSystem(linked),
		mEngine(engine),
		mDebugConsole(nullptr),
		mGfxListener(nullptr)
	{
		Colour::addStandardNamedColoursLua("data/namedColours.lua");
	}
	GameSystem::~GameSystem()
	{
		Colour::removeAllColours();
	}

	void GameSystem::setSize(int width, int height)
	{
		mLinkedSystem->setSize(width, height);
	}
	void GameSystem::setPosition(int x, int y)
	{
		mLinkedSystem->setPosition(x, y);
	}

	int GameSystem::getWidth() const
	{
		return mLinkedSystem->getWidth();
	}
	int GameSystem::getHeight() const
	{
		return mLinkedSystem->getHeight();
	}

	int GameSystem::getX() const
	{
		return mLinkedSystem->getX();
	}
	int GameSystem::getY() const
	{
		return mLinkedSystem->getY();
	}

	void GameSystem::setTitle(const char *title)
	{
		mLinkedSystem->setTitle(title);
	}
	const char *GameSystem::getTitle() const
	{
		return mLinkedSystem->getTitle();
	}

	void GameSystem::init()
	{
		GfxEngine *gfxEngine = GfxEngine::getEngine();
		gfxEngine ->init();
		
		mEngine->init();

		am::sfx::SfxEngine *sfxEngine = am::sfx::SfxEngine::getEngine();
		try
		{
			sfxEngine->init();
		}
		catch (const char *error)
		{
			stringstream ss;
			ss << "Unable to initialise sound engine: " << error;
			am_log("SFXERR", ss);
		}
		if (!sfxEngine->createContext())
		{
			am_log("SFXERR", "Error creating sound context");
			// TODO Figure out what should happen here.
		}
		
		mDebugConsole = new TextList();
		mDebugConsole->setName("DebugConsole");
		mDebugConsole->setMaxEntries(30);
		gfxEngine ->getDebugLayer()->addChild(mDebugConsole.get());

		mDebugConsole->setWidth(600.0f);
		mDebugConsole->setBaseFont("basic");
		mDebugConsole->setVisible(false);

		mGfxListener = new GfxLogListener(mDebugConsole);
		am::log::Logger::getMainLogger()->addLogListener(mGfxListener);
	}

	void GameSystem::reshape(int width, int height)
	{
		GfxEngine::getEngine()->reshape(width, height);
	}
	void GameSystem::update(float dt)
	{
		mEngine->update(dt);

		SfxEngine *engine = SfxEngine::getEngine();
		if (engine)
		{
			engine->update();
		}
		KeyboardManager::getManager()->onNewGameTick();
	}
	void GameSystem::display(float dt)
	{
		GfxEngine::getEngine()->display(dt);
	}
	void GameSystem::deinit()
	{
		if (mGfxListener)
		{
			delete mGfxListener;
			mGfxListener = nullptr;
		}
		Pathfinder::releasePathfinder();
	}

	void GameSystem::onMouseDown(am::ui::MouseButton mouseButton, int x, int y)
	{
		GfxEngine::getEngine()->getCursor()->setPosition(x, y);
		MouseManager::getManager()->onMouseDown(mouseButton, x, y);
	}
	void GameSystem::onMouseMove(am::ui::MouseButton mouseButton, int x, int y)
	{
		GfxEngine::getEngine()->getCursor()->setPosition(x, y);
		MouseManager::getManager()->onMouseMove(mouseButton, x, y);
	}
	void GameSystem::onMouseUp(am::ui::MouseButton mouseButton, int x, int y)
	{
		GfxEngine::getEngine()->getCursor()->setPosition(x, y);
		MouseManager::getManager()->onMouseUp(mouseButton, x, y);
	}
	void GameSystem::onKeyDown(int key, bool systemKey)
	{
		KeyboardManager::getManager()->onKeyDown(key, systemKey);
	}
	void GameSystem::onKeyUp(int key)
	{
		// 192 Currently is `
		if (key == 192)
		{
			mDebugConsole->setVisible(!mDebugConsole->isVisible());
		}

		KeyboardManager::getManager()->onKeyUp(key);
	}
	
	bool GameSystem::isProgramRunning() const
	{
		return mLinkedSystem->isProgramRunning();
	}
	void GameSystem::setProgramRunning(bool running)
	{
		mLinkedSystem->setProgramRunning(running);
	}

	bool GameSystem::isRunning() const
	{
		return mLinkedSystem->isRunning();
	}
	int GameSystem::startLoop()
	{
		return mLinkedSystem->startLoop();
	}
	void GameSystem::stopLoop()
	{
		mLinkedSystem->stopLoop();
	}

	void GameSystem::setCursorHidden(bool hide)
	{
		mLinkedSystem->setCursorHidden(hide);
	}
	void GameSystem::onCursorHiddenChange(bool hidden)
	{
		// If the OS cursor is hidden, we want to show our in game cursor.
		GfxEngine::getEngine()->setCursorHidden(!hidden);
	}
	bool GameSystem::isCursorHidden() const
	{
		return mLinkedSystem->isCursorHidden();
	}

	void GameSystem::setFullscreen(bool fullscreen)
	{
		mLinkedSystem->setFullscreen(fullscreen);
	}
	bool GameSystem::getFullscreen() const
	{
		return mLinkedSystem->getFullscreen();
	}

	ISystem *GameSystem::getLinkedSystem()
	{
		return mLinkedSystem;
	}
	Engine *GameSystem::getEngine()
	{
		return mEngine;
	}

	TextList *GameSystem::getDebugConsole()
	{
		return mDebugConsole.get();
	}

	bool GameSystem::isDirectory(const char *folderName)
	{
		if (mLinkedSystem)
		{
			return mLinkedSystem->isDirectory(folderName);
		}
		return false;
	}
	bool GameSystem::isFile(const char *filename)
	{
		if (mLinkedSystem)
		{
			return mLinkedSystem->isFile(filename);
		}
		return false;
	}
	bool GameSystem::createDirectory(const char *folderName)
	{
		if (mLinkedSystem)
		{
			return mLinkedSystem->createDirectory(folderName);
		}
		return false;
	}

}
}
