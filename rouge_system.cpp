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

#include "character.h"
#include "player_controller.h"

#include "game.h"
#include "screen.h"

namespace am {
namespace sys {

	RougeSystem *RougeSystem::sRougeSystem = NULL;

	RougeSystem *RougeSystem::createRougeSystem(ISystem *linked, Engine *engine)
	{
		sGameSystem = sRougeSystem = new RougeSystem(linked, engine);
		return sRougeSystem;
	}
	RougeSystem *RougeSystem::getRougeSystem()
	{
		return sRougeSystem;
	}

	RougeSystem::RougeSystem(ISystem *linked, Engine *engine) :
		GameSystem(linked, engine)
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

		Tile *grass = new Tile("grass");
		Tile *dirt = new Tile("dirt");

		TileSet *nature = new TileSet("am");
		nature->addTile(grass);
		nature->addTile(dirt);

		Engine *engine = Engine::getMainEngine();
		engine->addTileSet(nature);
	}

	void RougeSystem::reshape(int width, int height)
	{
		GameSystem::reshape(width, height);

		float fwidth = static_cast<float>(width);
		float fheight = static_cast<float>(height);
		if (mMainMenu.get())
		{
			mMainMenu->setWidth(fwidth);
			mMainMenu->setHeight(fheight);
		}
		if (mOptionsPanel.get())
		{
			mOptionsPanel->setWidth(fwidth);
			mOptionsPanel->setHeight(fheight);
		}
		if (mIngameMenu.get())
		{
			mIngameMenu->setWidth(fwidth);
			mIngameMenu->setHeight(fheight);
		}
	}

	void RougeSystem::onKeyUp(int key)
	{
		// 27 is currently escape.
		if (key == 27 && mEngine->getCurrentGame() != NULL)
		{
			mIngameMenu->setVisible(!mIngameMenu->isVisible());
			return;
		}
		Game *game = mEngine->getCurrentGame();;
		if (game)
		{
			if (key == 'A')
			{
				game->moveObjectToScreen(mPlayer.get(), "testScreen2", 100, 10, true);
				//game->getCamera()->followObject(mPlayer.get());
				//game->setCurrentScreen("testScreen2");
			}
			if (key == 'S')
			{
				//game->setCurrentScreen("testScreen");
				game->moveObjectToScreen(mPlayer.get(), "testScreen", 256, 256, true);
				///game->getCamera()->followObject(mPlayer.get());
			}
		}
		GameSystem::onKeyUp(key);
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

		// TODO: Reload testScreen!
		game->setCurrentScreen("testScreen");
		GfxEngine::getGfxEngine()->getGameLayer()->addChild(game->getGameLayer());

		mPlayer = new Character();
		mPlayer->setGraphic(new Sprite("mainChar/front"));
		mPlayer->setLocation(256, 256);
		game->addGameObject(mPlayer.get());

		PlayerController *controller = new PlayerController();
		mPlayer->setController(controller);

		game->getCamera()->followObject(mPlayer.get());

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
