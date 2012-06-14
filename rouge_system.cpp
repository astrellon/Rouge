#include "rouge_system.h"

#include "gfx/gfx_engine.h"
#include "gfx/gfx_layer.h"
#include "gfx/gfx_sprite.h"
#include "gfx/gfx_asset.h"

#include "ui_button.h"
#include "ui_checkbox.h"
#include "ui_main_menu.h"
#include "ui_options_panel.h"
#include "ui_ingame_menu.h"
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

		mMainMenu = new MainMenu(this);
		mGfxEngine->getUILayer()->addChild(mMainMenu.get());
		mMainMenu->setWidth(mGfxEngine->getScreenWidth());
		mMainMenu->setHeight(mGfxEngine->getScreenHeight());

		mOptionsPanel = new OptionsPanel(this);
		mGfxEngine->getUILayer()->addChild(mOptionsPanel.get());
		mOptionsPanel->setWidth(mGfxEngine->getScreenWidth());
		mOptionsPanel->setHeight(mGfxEngine->getScreenHeight());
		mOptionsPanel->setVisible(false);

		mIngameMenu = new IngameMenu(this);
		mGfxEngine->getUILayer()->addChild(mIngameMenu.get());
		mIngameMenu->setWidth(mGfxEngine->getScreenWidth());
		mIngameMenu->setHeight(mGfxEngine->getScreenHeight());
		mIngameMenu->setVisible(false);
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
		if (mIngameMenu.get())
		{
			mIngameMenu->setWidth(width);
			mIngameMenu->setHeight(height);
		}
	}

	void RougeSystem::onKeyUp(const bool *keys, int key)
	{
		// 27 is currently escape.
		if (key == 27 && mEngine->getCurrentGame() != NULL)
		{
			mIngameMenu->setVisible(!mIngameMenu->isVisible());
			return;
		}
		GameSystem::onKeyUp(keys, key);
	}

	void RougeSystem::quitGame()
	{
		setProgramRunning(false);
	}

	void RougeSystem::toMainMenu()
	{
		// Set current game to NULL and show main menu.
		Game *oldGame = mEngine->getCurrentGame();
		if (oldGame != NULL)
		{
			mGfxEngine->getGameLayer()->clear();
		}
		mEngine->setCurrentGame(NULL);
		mIngameMenu->setVisible(false);
		mMainMenu->setVisible(true);
	}
	void RougeSystem::showOptionsPanel()
	{
		mIngameMenu->setVisible(false);
		mMainMenu->setVisible(false);
		mOptionsPanel->setVisible(true);
	}
	void RougeSystem::closeOptionsPanel()
	{
		if (mEngine->getCurrentGame())
		{
			mIngameMenu->setVisible(true);
		}
		else
		{
			mMainMenu->setVisible(true);
		}
		mOptionsPanel->setVisible(false);
	}

	void RougeSystem::newGame()
	{
		Game *oldGame = mEngine->getCurrentGame();
		if (oldGame != NULL)
		{
			mGfxEngine->getGameLayer()->clear();
		}
		Game *game = new Game(mEngine, mGfxEngine);
		mEngine->setCurrentGame(game);

		//Handle<Screen> screen(new Screen(mGfxEngine, "testScreen"));

		//Handle<Sprite> sprite(new Sprite(mGfxEngine, "testScreen/background"));
		//screen->getBackground()->addChild(sprite.get());
		//Handle<Asset> asset(mGfxEngine->getAsset("testScreen/foreground"));
		
		//Handle<Sprite> foresprite(new Sprite(mGfxEngine, asset.get()));
		//Handle<Sprite> foresprite(new Sprite(mGfxEngine, "testScreen/foreground"));
		//Sprite *why = new Sprite(mGfxEngine, "testScreen/foreground");
		//screen->getForeground()->addChild(foresprite.get());
		
		//game->addScreen(screen.get());
		//game->setCurrentScreen("testScreen");
		mGfxEngine->getGameLayer()->addChild(game->getGameLayer());

		mMainMenu->setVisible(false);
		mIngameMenu->setVisible(false);
	}

	void RougeSystem::resumeGame()
	{
		mMainMenu->setVisible(false);
		mIngameMenu->setVisible(false);
	}

}
}
