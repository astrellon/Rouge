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
#include <ui/ui_debug_inspector.h>

#include <math/transform.h>

#include <math/transform.h>
#include <math/vector.h>

#include <sstream>

#include <base/handle.h>

#include <util/colour.h>

#include <sys/os_system.h>

using namespace std;
using namespace am::math;

namespace am {
namespace sys {

	GameSystem *GameSystem::sGameSystem = nullptr;

	GameSystem *GameSystem::createGameSystem(OsSystem *linked, Engine *engine)
	{
		sGameSystem = new GameSystem(linked, engine);
		return sGameSystem;
	}
	GameSystem *GameSystem::getGameSystem()
	{
		return sGameSystem;
	}

	GameSystem::GameSystem(OsSystem *linked, Engine *engine) :
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
		mDebugConsole->setMaxEntries(10000);
		mDebugConsole->setLinesToDisplay(80);
		gfxEngine ->getDebugLayer()->addChild(mDebugConsole);
		
		mDebugConsole->setWidth(600.0f);
		mDebugConsole->setBaseFont("default:basic");
		mDebugConsole->setVisible(false);

		mDebugInspector = new DebugInspector();
		mDebugInspector->setParentAnchor(X_LEFT, Y_TOP);
		mDebugInspector->setParentOffset(50.0f, 50.0f);
		mDebugInspector->setSize(250.0f, 300.0f);
		mDebugInspector->setVisible(false);
		gfxEngine ->getDebugLayer()->addChild(mDebugInspector);
		DebugInspector::setInspector(mDebugInspector);

		DebugInspector::getInspector()->setValue("test", "value");

		mGfxListener = new GfxLogListener(mDebugConsole);
		am::log::Logger::getMainLogger()->addLogListener(mGfxListener);

		//MouseManager *manager = MouseManager::getManager();
		//manager->addEventListener(MOUSE_OVER, this);
		//manager->addEventListener(MOUSE_OUT, this);

		mTooltip = new Tooltip();
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
		Pathfinder::releasePathfinder();
		DebugInspector::setInspector(nullptr);

		MouseManager *manager = MouseManager::getManager();
		manager->removeEventListener(am::ui::Mouse::MOUSE_OVER, this);
		manager->removeEventListener(am::ui::Mouse::MOUSE_OUT, this);
	}

	void GameSystem::onMouseDown(am::ui::Mouse::Button mouseButton, int x, int y)
	{
		GfxEngine::getEngine()->getCursor()->setPosition(x, y);
		MouseManager::getManager()->onMouseDown(mouseButton, x, y);
	}
	void GameSystem::onMouseMove(am::ui::Mouse::Button mouseButton, int x, int y)
	{
		GfxEngine::getEngine()->getCursor()->setPosition(x, y);
		MouseManager::getManager()->onMouseMove(mouseButton, x, y);
	}
	void GameSystem::onMouseUp(am::ui::Mouse::Button mouseButton, int x, int y)
	{
		GfxEngine::getEngine()->getCursor()->setPosition(x, y);
		MouseManager::getManager()->onMouseUp(mouseButton, x, y);
	}
	void GameSystem::onKeyDown(am::ui::Keyboard::Key key)
	{
		// 192 Currently is `
		if (key == am::ui::Keyboard::KEY_BACKTICK)
		{
			mDebugConsole->setVisible(!mDebugConsole->isVisible());
			mDebugInspector->setVisible(!mDebugInspector->isVisible());
		}
		// Page Up
		else if (key == am::ui::Keyboard::KEY_PAGEUP)
		{
			mDebugConsole->setScroll(mDebugConsole->getScroll() - 1);
		}
		// Page Down
		else if (key == am::ui::Keyboard::KEY_PAGEDOWN)
		{
			mDebugConsole->setScroll(mDebugConsole->getScroll() + 1);
		}
		KeyboardManager::getManager()->onKeyDown(key);
	}
	void GameSystem::onKeyUp(am::ui::Keyboard::Key key)
	{
		KeyboardManager::getManager()->onKeyUp(key);
	}
	void GameSystem::onKeyPress(char key)
	{
		KeyboardManager::getManager()->onKeyPress(key);
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

	OsSystem *GameSystem::getLinkedSystem()
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

	base::ReturnCode GameSystem::listDirectory(const char *folderName, ISystem::FolderEntryList &result)
	{
		if (mLinkedSystem)
		{
			return mLinkedSystem->listDirectory(folderName, result);
		}
		return NO_LINKED_SYSTEM;
	}

	void GameSystem::onEvent(MouseEvent *e)
	{
		if (!e)
		{
			return;
		}

		/*if (e->getMouseEventType() == MOUSE_OVER && e->getTarget())
		{
			const char *tooltip = e->getTarget()->getTooltip();
			if (tooltip && tooltip[0] != '\0')
			{
				mTooltip->setText(e->getTarget()->getTooltip());
				mTooltip->setDetailedText(e->getTarget()->getDetailedTooltip());
				mTooltip->setPosition(e->getMouseX(), e->getMouseY());
				mTooltip->active();
			}
		}
		else
		{
			mTooltip->hide();
		}*/
	}

	Tooltip *GameSystem::getDefaultTooltip() const
	{
		return mTooltip;
	}

}
}
