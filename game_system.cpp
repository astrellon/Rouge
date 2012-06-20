#include "game_system.h"

#include "engine.h"

#include "gfx/gfx_sprite.h"
#include "gfx/gfx_text_field.h"
#include "gfx/gfx_layer.h"
#include "gfx/gfx_font.h"
#include "gfx/gfx_asset.h"
#include "gfx/scale_nine.h"
#include "gfx/gfx_text_list.h"
#include "gfx/gfx_log_listener.h"
#include "gfx/gfx_engine.h"

#include "logger.h"

#include "mouse_manager.h"
#include "keyboard_manager.h"

#include "transform.h"

#include "transform.h"
#include "vector.h"

#include <sstream>

#include "ui_button.h"
#include "ui_checkbox.h"
#include "ui_label.h"
#include "ui_options_panel.h"
#include "ui_main_menu.h"

#include "handle.h"

using namespace std;
using namespace am::math;

namespace am {
namespace sys {

	GameSystem *GameSystem::sGameSystem = NULL;

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
		mDebugConsole(NULL)
	{

	}
	GameSystem::~GameSystem()
	{
		
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
		GfxEngine *gfxEngine = GfxEngine::getGfxEngine();
		gfxEngine ->init();
		
		mEngine->init();
		
		mDebugConsole = new TextList();
		mDebugConsole->setMaxEntries(30);
		gfxEngine ->getDebugLayer()->addChild(mDebugConsole.get());

		mDebugConsole->setWidth(600.0f);
		mDebugConsole->setBaseFont("basic");

		GfxLogListener *listener = new GfxLogListener(mDebugConsole.get());
		am::log::Logger::getMainLogger()->addLogListener(listener);
	}

	void GameSystem::reshape(int width, int height)
	{
		GfxEngine::getGfxEngine()->reshape(width, height);
	}
	void GameSystem::update(float dt)
	{
		mEngine->update(dt);
	}
	void GameSystem::display(float dt)
	{
		GfxEngine::getGfxEngine()->display(dt);
	}
	void GameSystem::deinit()
	{
		GfxEngine::getGfxEngine()->deinit();
		mEngine->deinit();
	}

	void GameSystem::onMouseDown(am::ui::MouseButton mouseButton, int x, int y)
	{
		GfxEngine::getGfxEngine()->getCursor()->setPosition(x, y);
		MouseManager::getManager()->onMouseDown(mouseButton, x, y);
	}
	void GameSystem::onMouseMove(am::ui::MouseButton mouseButton, int x, int y)
	{
		GfxEngine::getGfxEngine()->getCursor()->setPosition(x, y);
		MouseManager::getManager()->onMouseMove(mouseButton, x, y);
	}
	void GameSystem::onMouseUp(am::ui::MouseButton mouseButton, int x, int y)
	{
		GfxEngine::getGfxEngine()->getCursor()->setPosition(x, y);
		MouseManager::getManager()->onMouseUp(mouseButton, x, y);
	}
	void GameSystem::onKeyDown(int key)
	{
		KeyboardManager::getManager()->onKeyDown(key);
	}
	void GameSystem::onKeyUp(int key)
	{
		stringstream ss;
		ss << "Key up: " << key << " (" << (char)key << ")";
		am_log("KEY", ss.str().c_str());

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
		GfxEngine::getGfxEngine()->setCursorHidden(!hidden);
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

}
}
