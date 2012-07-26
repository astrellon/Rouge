#include "rouge_system.h"

#include <gfx/gfx_engine.h>
#include <gfx/gfx_layer.h>
#include <gfx/gfx_sprite.h>
#include <gfx/gfx_asset.h>
#include <gfx/gfx_text_field.h>
#include <gfx/gfx_text_style.h>
#include <gfx/gfx_texture.h>
#include <gfx/gfx_particle_system.h>
#include <gfx/gfx_text_style_selector.h>
#include <gfx/gfx_node.h>
#include <gfx/gfx_text_field2.h>

#include <ui/ui_button.h>
#include <ui/ui_checkbox.h>
#include <ui/ui_main_menu.h>
#include <ui/ui_options_panel.h>
#include <ui/ui_ingame_menu.h>
#include <ui/ui_game_hud.h>
#include <ui/ui_panel.h>
#include <ui/ui_image.h>
#include <ui/ui_editor_hud.h>

#include <game/character.h>
#include <game/player_controller.h>

#include <game/game.h>
#include <game/engine.h>

#include <game/tile.h>
#include <game/tile_instance.h>
#include <game/tile_set.h>
#include <game/tile_type.h>

#include <game/map.h>

#include <util/json_value.h>
#include <util/text_tokeniser.h>
#include <util/utils.h>
#include <log/logger.h>

#include <game/stat_range.h>

#include <game/stats.h>
#include <game/stats_common.h>

#include <game/item.h>
#include <game/item_instance.h>

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
		TileType::loadStandardTileTypes("data/tileTypes.ssff");
		TextStyle::loadStyles("data/textStyles.ssff");

		GfxEngine *gfxEngine = GfxEngine::getEngine();
		float screenWidth = static_cast<float>(gfxEngine->getScreenWidth());
		float screenHeight = static_cast<float>(gfxEngine->getScreenHeight());
		mMainMenu = new MainMenu(this);
		gfxEngine->getUILayer()->addChild(mMainMenu.get());
		mMainMenu->setSize(screenWidth, screenHeight);
		
		mOptionsPanel = new OptionsPanel(this);
		gfxEngine->getUILayer()->addChild(mOptionsPanel.get());
		mOptionsPanel->setSize(screenWidth, screenHeight);
		mOptionsPanel->setVisible(false);

		mIngameMenu = new IngameMenu(this);
		gfxEngine->getUILayer()->addChild(mIngameMenu.get());
		mIngameMenu->setSize(screenWidth, screenHeight);
		mIngameMenu->setVisible(false);

		mGameHud = new GameHud();
		gfxEngine->getUILayer()->addChild(mGameHud.get());
		mGameHud->setSize(screenWidth, screenHeight);
		mGameHud->setVisible(false);

		mPausedImage = new Image("paused");
		mPausedImage->setParentAnchor(X_CENTER, Y_CENTER);
		mPausedImage->setAnchor(X_CENTER, Y_CENTER);

		mEditorHud = new EditorHud();
		mEditorHud->setSize(screenWidth, screenHeight);
		mEditorHud->setVisible(false);
		
		Engine *engine = Engine::getEngine();
		engine->setGameHud(mGameHud);

		setCurrentMenu(mMainMenu);
		
		Handle<TextField2> field2(new TextField2());
		field2->setText("Hello <character special='true'>Melli</character> how are you?\nWould you like this <item>Dress</item>?");
		field2->parseRawText();
		field2->setInteractive(true);
		field2->setPosition(400, 100);
		gfxEngine->getUILayer()->addChild(field2);

		/*Handle<ParticleSystem> test(new ParticleSystem());
		test->setParticleAsset("testParticle");
		test->setMaxAge(10.0f);
		test->setMaxParticles(1000);
		test->setParticlesPerSecond(20.0f);
		test->setAcceleration(0.0f, 20.0f);
		test->setPower(30.0f);
		test->setPowerVariation(15.0f);
		test->setDirectionVariation(0);
		test->setPosition(400, 100);
		gfxEngine->getUILayer()->addChild(test);*/
	}
	
	void RougeSystem::reshape(int width, int height)
	{
		GameSystem::reshape(width, height);

		float fwidth = static_cast<float>(width);
		float fheight = static_cast<float>(height);
		if (mMainMenu.get())
		{
			mMainMenu->setSize(fwidth, fheight);
		}
		if (mOptionsPanel.get())
		{
			mOptionsPanel->setSize(fwidth, fheight);
		}
		if (mIngameMenu.get())
		{
			mIngameMenu->setSize(fwidth, fheight);
		}
		if (mGameHud.get())
		{
			mGameHud->setSize(fwidth, fheight);
		}
		if (mEditorHud.get())
		{
			mEditorHud->setSize(fwidth, fheight);
		}
	}

	void RougeSystem::togglePause()
	{
		mPausedGame = !mPausedGame;
		checkPaused();
	}

	void RougeSystem::toggleInGameMenu()
	{
		setCurrentMenu(mIngameMenu->isVisible() ? NULL : mIngameMenu);
		checkPaused();
	}

	void RougeSystem::checkPaused()
	{
		if (isPaused())
		{
			mGameHud->setInteractive(false);
			GfxEngine::getEngine()->getUILayer()->addChild(mPausedImage.get());
			mPausedImage->setVisible(!mIngameMenu->isVisible());
		}
		else
		{
			mGameHud->setInteractive(true);
			GfxEngine::getEngine()->getUILayer()->removeChild(mPausedImage.get());
		}
	}

	bool RougeSystem::isPaused()
	{
		return mPausedGame || mIngameMenu->isVisible();
	}

	void RougeSystem::pauseGame(bool paused)
	{
		mPausedGame = true;
		checkPaused();
	}

	void RougeSystem::resumeGame()
	{
		mPausedGame = false;
		mIngameMenu->setVisible(false);
		checkPaused();
	}

	void RougeSystem::onKeyUp(int key)
	{
		// 27 is currently escape.
		if (key == 27 && mEngine->getCurrentGame() != NULL)
		{
			toggleInGameMenu();
			return;
		}
		// 19 is currently pause.
		if (key == 19 && mEngine->getCurrentGame() != NULL)
		{
			togglePause();
			return;
		}
		if (key == 'Z')
		{
			GfxEngine::getEngine()->reloadAsset("bigButton");
		}
		Game *game = mEngine->getCurrentGame();
		if (game)
		{
			if (key == 'A')
			{
				game->moveObjectToMapGrid(mPlayer.get(), "testMap2", 3, 1, true);
			}
			if (key == 'S')
			{
				game->moveObjectToMapGrid(mPlayer.get(), "testMap", 1, 3, true);
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
		MouseManager::getManager()->clearCurrentlyFiring();
		// Set current game to NULL and show main menu.
		Game *oldGame = mEngine->getCurrentGame();
		if (oldGame != NULL)
		{
			GfxEngine::getEngine()->getGameLayer()->clear();
		}
		mEngine->setCurrentGame(NULL);
		//mIngameMenu->setVisible(false);
		//mMainMenu->setVisible(true);
		setCurrentMenu(mMainMenu);
		mGameHud->setVisible(false);

	}
	void RougeSystem::showOptionsPanel()
	{
		//mIngameMenu->setVisible(false);
		//mMainMenu->setVisible(false);
		//mOptionsPanel->setVisible(true);
		setCurrentMenu(mOptionsPanel);
	}
	void RougeSystem::closeOptionsPanel()
	{
		if (mEngine->getCurrentGame())
		{
			//mIngameMenu->setVisible(true);
			setCurrentMenu(mIngameMenu);
		}
		else
		{
			setCurrentMenu(mMainMenu);
			//mMainMenu->setVisible(true);
		}
		//mOptionsPanel->setVisible(false);

	}

	void RougeSystem::newGame()
	{
		MouseManager::getManager()->clearCurrentlyFiring();
		Game *oldGame = mEngine->getCurrentGame();
		if (oldGame != NULL)
		{
			GfxEngine::getEngine()->getGameLayer()->clear();
		}

		Game *game = new Game(mEngine);
		mEngine->setCurrentGame(game);

		mPausedGame = false;

		game->setCurrentMap("testMap");
		GfxEngine::getEngine()->getGameLayer()->addChild(game->getGameLayer());

		mPlayer = new Character();
		mPlayer->addPassibleType(TileType::getTileType("land"));
		mPlayer->setGraphic(new Sprite("mainChar/front"));
		mPlayer->setGridLocation(2, 1);
		game->addGameObject(mPlayer.get());

		PlayerController *controller = new PlayerController();
		mPlayer->setController(controller);

		game->getCamera()->followObject(mPlayer.get());

		GameHud *gameHud = Engine::getEngine()->getGameHud();
		if (gameHud)
		{
			gameHud->getCharacterScreen()->setCharacter(mPlayer);
		}

		//mMainMenu->setVisible(false);
		//mIngameMenu->setVisible(false);
		setCurrentMenu(NULL);
		mGameHud->setVisible(true);
	}

	void RougeSystem::setCurrentMenu(UIComponent *menu)
	{
		if (mCurrentMenu.get())
		{
			mCurrentMenu->setVisible(false);
		}
		mCurrentMenu = menu;
		if (mCurrentMenu.get())
		{
			mCurrentMenu->setVisible(true);
		}
	}

}
}
