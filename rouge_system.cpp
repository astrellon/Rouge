#include "rouge_system.h"

#include "gfx/gfx_engine.h"
#include "gfx/gfx_layer.h"
#include "gfx/gfx_sprite.h"

#include "ui_button.h"
#include "ui_checkbox.h"
#include "ui_main_menu.h"
#include "ui_options_panel.h"
#include "engine.h"

#include "game.h"
#include "screen.h"

namespace am {
namespace sys {

	RougeSystem *RougeSystem::sRougeSystem = NULL;

	RougeSystem *RougeSystem::createRougeSystem(ISystem *linked, Engine *engine, GfxEngine *gfxEngine,
			MouseManager *mouseManager)
	{
		sGameSystem = sRougeSystem = new RougeSystem(linked, engine, gfxEngine, mouseManager);
		return sRougeSystem;
	}
	RougeSystem *RougeSystem::getRougeSystem()
	{
		return sRougeSystem;
	}

	RougeSystem::RougeSystem(ISystem *linked, Engine *engine, GfxEngine *gfxEngine,
		MouseManager *mouseManager) :
		GameSystem(linked, engine, gfxEngine, mouseManager)
	{

	}
	RougeSystem::~RougeSystem()
	{
	}

	void RougeSystem::init()
	{
		GameSystem::init();

		mMainMenu = new MainMenu(mGfxEngine);
		mMainMenu->addEventListener("quit", this);
		mMainMenu->addEventListener("options", this);
		mMainMenu->addEventListener("new_game", this);
		mGfxEngine->getUILayer()->addChild(mMainMenu.get());
		mMainMenu->setWidth(mGfxEngine->getScreenWidth());
		mMainMenu->setHeight(mGfxEngine->getScreenHeight());

		mOptionsPanel = new OptionsPanel(mGfxEngine);
		mOptionsPanel->addEventListener("close_options", this);
		mGfxEngine->getUILayer()->addChild(mOptionsPanel.get());
		mOptionsPanel->setWidth(mGfxEngine->getScreenWidth());
		mOptionsPanel->setHeight(mGfxEngine->getScreenHeight());
		mOptionsPanel->setVisible(false);
	}

	void RougeSystem::reshape(int width, int height)
	{
		GameSystem::reshape(width, height);

		if (mMainMenu.get())
		{
			mMainMenu->setWidth(width);
			mMainMenu->setHeight(height);
		}
		if (mOptionsPanel.get())
		{
			mOptionsPanel->setWidth(width);
			mOptionsPanel->setHeight(height);
		}
	}

	void RougeSystem::onEvent(am::ui::Event *e)
	{
		if (e->getType().compare("quit") == 0)
		{
			setProgramRunning(false);
			return;
		}
		if (e->getType().compare("options") == 0)
		{
			mMainMenu->setVisible(false);
			mOptionsPanel->setVisible(true);
			return;
		}
		if (e->getType().compare("close_options") == 0)
		{
			mMainMenu->setVisible(true);
			mOptionsPanel->setVisible(false);
			return;
		}
		if (e->getType().compare("new_game") == 0)
		{
			newGame();
			return;
		}

		GameSystem::onEvent(e);
	}

	void RougeSystem::newGame()
	{
		Game *oldGame = mEngine->getCurrentGame();
		if (oldGame != NULL)
		{
			mGfxEngine->getGameLayer()->clear();
			delete oldGame;
		}
		Game *game = new Game(mEngine, mGfxEngine);
		mEngine->setCurrentGame(game);

		Screen *screen = new Screen(mGfxEngine, "testScreen");

		screen->getBackground()->addChild(new Sprite(mGfxEngine, "background"));
		screen->getForeground()->addChild(new Sprite(mGfxEngine, "foreground"));

		game->addScreen(screen);
		game->setCurrentScreen("testScreen");
		mGfxEngine->getGameLayer()->addChild(game->getGameLayer());
	}

}
}
