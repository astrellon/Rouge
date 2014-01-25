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
#include <gfx/gfx_camera.h>

#include <sfx/sfx_engine.h>

#include <log/logger.h>

#include <ui/mouse_manager.h>
#include <ui/keyboard_manager.h>
#include <ui/ui_debug_inspector.h>

#include <math/transform.h>

#include <math/transform.h>
#include <math/vector.h>

#include <sstream>

#include <base/handle.h>

#include <util/colour.h>

#include <sys/os_system.h>

namespace am {
namespace sys {

	GameSystem *GameSystem::sGameSystem = nullptr;

	GameSystem *GameSystem::createGameSystem(OsSystem *linked, game::Engine *engine)
	{
		sGameSystem = new GameSystem(linked, engine);
		return sGameSystem;
	}
	GameSystem *GameSystem::getGameSystem()
	{
		return sGameSystem;
	}

	GameSystem::GameSystem(OsSystem *linked, game::Engine *engine) :
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
		gfx::GfxEngine *gfxEngine = gfx::GfxEngine::getEngine();
		gfxEngine ->init();
		
		mEngine->init();

		sfx::SfxEngine *sfxEngine = am::sfx::SfxEngine::getEngine();
		try
		{
			sfxEngine->init();
		}
		catch (const char *error)
		{
			std::stringstream ss;
			ss << "Unable to initialise sound engine: " << error;
			am_log("SFXERR", ss);
		}
		if (!sfxEngine->createContext())
		{
			am_log("SFXERR", "Error creating sound context");
			// TODO Figure out what should happen here.
		}
		
		mDebugConsole = new gfx::TextList();
		mDebugConsole->setName("DebugConsole");
		mDebugConsole->setMaxEntries(10000);
		mDebugConsole->setLinesToDisplay(80);
		gfxEngine ->getDebugLayer()->addChild(mDebugConsole);
		
		mDebugConsole->setWidth(600.0f);
		mDebugConsole->setBaseFont("default:basic");
		mDebugConsole->setVisible(false);

		mDebugInspector = new ui::DebugInspector();
		mDebugInspector->setParentAnchor(ui::X_LEFT, ui::Y_TOP);
		mDebugInspector->setParentOffset(50.0f, 50.0f);
		mDebugInspector->setSize(250.0f, 300.0f);
		mDebugInspector->setVisible(false);
		gfxEngine ->getDebugLayer()->addChild(mDebugInspector);
		ui::DebugInspector::setInspector(mDebugInspector);

		ui::DebugInspector::getInspector()->setValue("test", "value");

		mGfxListener = new gfx::GfxLogListener(mDebugConsole);
		am::log::Logger::getMainLogger()->addLogListener(mGfxListener);

		//MouseManager *manager = MouseManager::getManager();
		//manager->addEventListener(MOUSE_OVER, this);
		//manager->addEventListener(MOUSE_OUT, this);

		mTooltip = new gfx::Tooltip();
	}

	void GameSystem::reshape(int width, int height)
	{
		gfx::GfxEngine::getEngine()->reshape(width, height);
	}
	void GameSystem::update(float dt)
	{
		mEngine->update(dt);

		sfx::SfxEngine *engine = sfx::SfxEngine::getEngine();
		if (engine)
		{
			engine->update();
		}
		ui::KeyboardManager::getManager()->onNewGameTick();
	}
	void GameSystem::display(float dt)
	{
		game::Game *game = game::Engine::getGame();
		if (game)
		{
			game->getCamera()->update(0.0f);
		}
		gfx::GfxEngine::getEngine()->display(dt);
	}
	void GameSystem::deinit()
	{
		game::Pathfinder::releasePathfinder();
		ui::DebugInspector::setInspector(nullptr);

		ui::MouseManager *manager = ui::MouseManager::getManager();
		manager->removeEventListener(ui::Mouse::MOUSE_OVER, this);
		manager->removeEventListener(ui::Mouse::MOUSE_OUT, this);
	}

	void GameSystem::onMouseDown(ui::Mouse::Button mouseButton, int x, int y)
	{
		gfx::GfxEngine::getEngine()->getCursor()->setPosition(x, y);
		ui::MouseManager::getManager()->onMouseDown(mouseButton, x, y);
	}
	void GameSystem::onMouseMove(int x, int y)
	{
		gfx::GfxEngine::getEngine()->getCursor()->setPosition(x, y);
		ui::MouseManager::getManager()->onMouseMove(x, y);
	}
	void GameSystem::onMouseUp(ui::Mouse::Button mouseButton, int x, int y)
	{
		gfx::GfxEngine::getEngine()->getCursor()->setPosition(x, y);
		ui::MouseManager::getManager()->onMouseUp(mouseButton, x, y);
	}
	void GameSystem::onKeyDown(ui::Keyboard::Key key)
	{
		ui::KeyboardManager::getManager()->onKeyDown(key);
	}
	void GameSystem::onKeyUp(ui::Keyboard::Key key)
	{
		ui::KeyboardManager::getManager()->onKeyUp(key);
	}
	void GameSystem::onKeyPress(char key)
	{
        std::stringstream ss;
        ss << "Key: " << (int)key << ", " << (int)ui::Keyboard::KEY_PAGEUP;
        am_log("KEY", ss);

		if (key == ui::Keyboard::KEY_BACKTICK)
		{
			mDebugConsole->setVisible(!mDebugConsole->isVisible());
			mDebugInspector->setVisible(!mDebugInspector->isVisible());
		}
		// Page Up
		else if (key == ui::Keyboard::KEY_PAGEUP)
		{
			mDebugConsole->setScroll(mDebugConsole->getScroll() - 1);
		}
		// Page Down
		else if (key == ui::Keyboard::KEY_PAGEDOWN)
		{
			mDebugConsole->setScroll(mDebugConsole->getScroll() + 1);
		}
		ui::KeyboardManager::getManager()->onKeyPress(key);
	}
	
	bool GameSystem::isProgramRunning() const
	{
		return mLinkedSystem->isProgramRunning();
	}
	void GameSystem::setProgramRunning(bool running)
	{
		mLinkedSystem->setProgramRunning(running);
	}

	int GameSystem::startLoop(int argc, char **argv)
	{
		return mLinkedSystem->startLoop(argc, argv);
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
		gfx::GfxEngine::getEngine()->setCursorHidden(!hidden);
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

	OsSystem *GameSystem::getLinkedSystem()
	{
		return mLinkedSystem;
	}
	game::Engine *GameSystem::getEngine()
	{
		return mEngine;
	}

	gfx::TextList *GameSystem::getDebugConsole()
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

	base::ReturnCode GameSystem::listDirectory(const char *folderName, ISystem::FolderEntryList &result)
	{
		if (mLinkedSystem)
		{
			return mLinkedSystem->listDirectory(folderName, result);
		}
		return base::NO_LINKED_SYSTEM;
	}

	void GameSystem::onEvent(ui::MouseEvent *e)
	{
		if (!e)
		{
			return;
		}
	}

	gfx::Tooltip *GameSystem::getDefaultTooltip() const
	{
		return mTooltip;
	}

}
}
