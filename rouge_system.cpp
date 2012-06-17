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

	RougeSystem *RougeSystem::createRougeSystem(ISystem *linked, Engine *engine, MouseManager *mouseManager)
	{
		sGameSystem = sRougeSystem = new RougeSystem(linked, engine, mouseManager);
		return sRougeSystem;
	}
	RougeSystem *RougeSystem::getRougeSystem()
	{
		return sRougeSystem;
	}

	RougeSystem::RougeSystem(ISystem *linked, Engine *engine, MouseManager *mouseManager) :
		GameSystem(linked, engine, mouseManager)
	{

	}
	RougeSystem::~RougeSystem()
	{
	}

	void RougeSystem::init()
	{
		GameSystem::init();

		GfxEngine *gfxEngine = GfxEngine::getGfxEngine();
		float screenWidth = static_cast<float>(gfxEngine->getScreenWidth());
		float screenHeight = static_cast<float>(gfxEngine->getScreenHeight());
		mMainMenu = new MainMenu(this);
		gfxEngine->getUILayer()->addChild(mMainMenu.get());
		mMainMenu->setWidth(screenWidth);
		mMainMenu->setHeight(screenHeight);
		
		mOptionsPanel = new OptionsPanel(this);
		gfxEngine->getUILayer()->addChild(mOptionsPanel.get());
		mOptionsPanel->setWidth(screenWidth);
		mOptionsPanel->setHeight(screenHeight);
		mOptionsPanel->setVisible(false);

		mIngameMenu = new IngameMenu(this);
		gfxEngine->getUILayer()->addChild(mIngameMenu.get());
		mIngameMenu->setWidth(screenWidth);
		mIngameMenu->setHeight(screenHeight);
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
			GfxEngine::getGfxEngine()->getGameLayer()->clear();
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
			GfxEngine::getGfxEngine()->getGameLayer()->clear();
		}
		Game *game = new Game(mEngine);
		mEngine->setCurrentGame(game);

		game->setCurrentScreen("testScreen");
		GfxEngine::getGfxEngine()->getGameLayer()->addChild(game->getGameLayer());

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
