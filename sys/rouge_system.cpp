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
#include <gfx/gfx_scrollbar.h>
#include <gfx/gfx_button.h>
#include <gfx/gfx_tooltip_item.h>

#include <ui/ui_text_button.h>
#include <ui/ui_checkbox.h>
#include <ui/ui_main_menu.h>
#include <ui/ui_options_panel.h>
#include <ui/ui_ingame_menu.h>
#include <ui/ui_game_hud.h>
#include <ui/ui_panel.h>
#include <ui/ui_image.h>
#include <ui/ui_editor_hud.h>
#include <ui/ui_inventory_renderer.h>
#include <ui/ui_body_part_renderer.h>
#include <ui/ui_text_input.h>
#include <ui/ui_dialogue_box.h>
#include <ui/ui_store_screen.h>

#include <game/character.h>
#include <game/race.h>
#include <game/player_controller.h>
#include <game/player_hand.h>
#include <game/game.h>
#include <game/engine.h>
#include <game/dialogue.h>
#include <game/dialogue_choice.h>
#include <game/tile.h>
#include <game/tile_instance.h>
#include <game/tile_set.h>
#include <game/tile_type.h>
#include <game/inventory.h>
#include <game/map.h>
#include <game/string_pool.h>
#include <game/door.h>

#include <util/text_tokeniser.h>
#include <util/utils.h>
#include <log/logger.h>

#include <game/stat_range.h>

#include <game/stats.h>
#include <game/stats_common.h>

#include <game/item.h>
#include <game/item_instance.h>

#include <lua/wrappers/game/lua_map.h>

#include <sfx/sfx_isound.h>
#include <sfx/sfx_engine.h>
#include <sfx/sfx_source_point.h>
#include <sfx/sfx_source_area.h>

namespace am {
namespace sys {

	RougeSystem *RougeSystem::sRougeSystem = nullptr;

	RougeSystem *RougeSystem::createRougeSystem(OsSystem *linked, game::Engine *engine)
	{
		sGameSystem = sRougeSystem = new RougeSystem(linked, engine);
		return sRougeSystem;
	}
	RougeSystem *RougeSystem::getRougeSystem()
	{
		return sRougeSystem;
	}

	RougeSystem::RougeSystem(OsSystem *linked, game::Engine *engine) :
		GameSystem(linked, engine),
		mPlayerHand(nullptr),
		mInDialogue(false)
	{

	}
	RougeSystem::~RougeSystem()
	{
		/*if (mPlayer)
		{
			mPlayer->removeEventListener("dialogue", this);
		}
		Game *game = Engine::getGame();
		if (game)
		{
			game->removeEventListener("startGame", this);
		}*/
	}

	void RougeSystem::init()
	{
		GameSystem::init();
		gfx::TextStyle::loadStylesLua("data/textStyles.lua");

		gfx::GfxEngine *gfxEngine = gfx::GfxEngine::getEngine();
		float screenWidth = static_cast<float>(gfxEngine->getScreenWidth());
		float screenHeight = static_cast<float>(gfxEngine->getScreenHeight());
		mMainMenu = new ui::MainMenu(this);
		gfxEngine->getUILayer()->addChild(mMainMenu.get());
		mMainMenu->setSize(screenWidth, screenHeight);
		
		mOptionsPanel = new ui::OptionsPanel(this);
		gfxEngine->getUILayer()->addChild(mOptionsPanel.get());
		mOptionsPanel->setSize(screenWidth, screenHeight);
		mOptionsPanel->setVisible(false);

		mIngameMenu = new ui::IngameMenu(this);
		gfxEngine->getUILayer()->addChild(mIngameMenu.get());
		mIngameMenu->setSize(screenWidth, screenHeight);
		mIngameMenu->setVisible(false);

		mGameHud = new ui::GameHud();
		gfxEngine->getUILayer()->addChild(mGameHud.get());
		mGameHud->setSize(screenWidth, screenHeight);
		mGameHud->setVisible(false);

		mEditorHud = new ui::EditorHud();
		gfxEngine->getUILayer()->addChild(mEditorHud);
		mEditorHud->setSize(screenWidth, screenHeight);
		mEditorHud->setVisible(false);

		mPausedImage = new ui::Image("ui:paused");
		mPausedImage->setParentAnchor(ui::X_CENTER, ui::Y_CENTER);
		mPausedImage->setAnchor(ui::X_CENTER, ui::Y_CENTER);

		Engine *engine = Engine::getEngine();
		engine->setGameHud(mGameHud);
		engine->setEditorHud(mEditorHud);

		setCurrentMenu(mMainMenu);

		mPlayerHand = new PlayerHand();
		PlayerHand::setPlayerHand(mPlayerHand);

		mEngine->loadLuaEngine("data/engine.lua");

		sfx::SfxEngine *sfxEngine = sfx::SfxEngine::getEngine();
		sfx::ISound *bgm = sfxEngine->loadStream("18765__reinsamba__evening-in-the-forest.ogg");
		//sfx::ISound *bgm = sfxEngine->loadStream("210074__erokia__drums-piano-loop-3.wav");
		sfx::SourcePoint *bgmSource = new sfx::SourcePoint(bgm);
		bgmSource->setSourceRelative(true);
		bgmSource->setGain(0.2f);
		bgmSource->play();

		mItemTooltip = new gfx::TooltipItem();

		//ISystem::FolderEntryList result;
		//listDirectory("C:\\git\\Rouge\\data", result);
		/*Sprite *test = new Sprite("tiles/testTransition");
		test->setPosition(50, 50);
		test->setTextureFrame(31);
		gfxEngine->getRootLayer()->addChild(test);*/

		/*
		base::Handle<TextInput> input(new TextInput());
		input->setFocus(true);
		input->setParentOffset(200, 50);
		gfxEngine->getUILayer()->addChild(input);*/

		
		/*base::Handle<Tooltip> tip(new Tooltip("Tooooltip", "<title>Longer </title>otoltip"));
		tip->show();
		tip->setPosition(400, 100);*/
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
		setCurrentMenu(mIngameMenu->isVisible() ? nullptr : mIngameMenu);
		checkPaused();
	}

	void RougeSystem::checkPaused()
	{
		if (isPaused())
		{
			mGameHud->setInteractive(false);
			gfx::GfxEngine::getEngine()->getUILayer()->addChild(mPausedImage.get());
			mPausedImage->setVisible(!mIngameMenu->isVisible());
		}
		else
		{
			mGameHud->setInteractive(true);
			gfx::GfxEngine::getEngine()->getUILayer()->removeChild(mPausedImage.get());
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

	void RougeSystem::saveGame(const char *savename)
	{
		if (savename == nullptr || savename[0] == '\0')
		{
			return;
		}

		game::Game *game = game::Engine::getGame();
		if (game)
		{
			game->saveGame(savename);
		}
		else
		{
			am_log("SAVEERR", "Cannot save when no game is in progress.");
		}
	}

	void RougeSystem::loadGame(const char *savename)
	{
		if (savename == nullptr || savename[0] == '\0')
		{
			return;
		}

		ui::MouseManager::getManager()->clearCurrentlyFiring();
		game::Game *oldGame = mEngine->getCurrentGame();
		if (oldGame != nullptr)
		{
			gfx::GfxEngine::getEngine()->getGameLayer()->clear();
		}

		if (mEngine->loadGame(savename))
		{
			game::Game *game = game::Engine::getGame();
			if (game)
			{
				if (game->hasStarted())
				{
					startGame();
				}
				game->addEventListener("startGame", this);
			}
		}
	}

	void RougeSystem::onKeyUp(ui::Keyboard::Key key)
	{
		// 27 is currently escape.
		if (mInDialogue && key == ui::Keyboard::KEY_ESC)
		{
			if (mPlayer->getDialogueComp())
			{
				mPlayer->getDialogueComp()->talkTo(mPlayer->getDialogueComp()->getTalkingTo(), nullptr);
			}
			return;
		}
		if (key == ui::Keyboard::KEY_ESC && mEngine->getCurrentGame() != nullptr)
		{
			toggleInGameMenu();
			return;
		}
		// 19 is currently pause.
		if (key == ui::Keyboard::KEY_PAUSE && mEngine->getCurrentGame() != nullptr)
		{
			togglePause();
			return;
		}
		if (key == ui::Keyboard::KEY_Z)
		{
			gfx::GfxEngine::getEngine()->reloadAsset("bigButton");
		}
		game::Game *game = mEngine->getCurrentGame();
		if (game)
		{
			if (key == ui::Keyboard::KEY_A)
			{
				game->moveObjectToMapGrid(mPlayer.get(), "testMap_2", 3, 1, true);
			}
			if (key == ui::Keyboard::KEY_S)
			{
				game->moveObjectToMapGrid(mPlayer.get(), "testMap", 1, 3, true);
			}
		}
		GameSystem::onKeyUp(key);
	}
	void RougeSystem::onEvent(ui::DialogueEvent *e)
	{
		mInDialogue = e->getDialogue() != nullptr;
	}

	void RougeSystem::quitGame()
	{
		setProgramRunning(false);
	}

	void RougeSystem::toMainMenu()
	{
		ui::MouseManager::getManager()->clearCurrentlyFiring();
		// Set current game to nullptr and show main menu.
		/*if (mPlayer)
		{
			mPlayer->removeEventListener("dialogue", this);
			mPlayer = nullptr;
		}*/
		game::Game *oldGame = mEngine->getCurrentGame();
		if (oldGame != nullptr)
		{
			gfx::GfxEngine::getEngine()->getGameLayer()->clear();
			oldGame->removeEventListener("startGame", this);
			oldGame->deinit();
		}
		mPlayer = nullptr;
		//mGame = nullptr;
		
		mEngine->setCurrentGame(nullptr);
		setCurrentMenu(mMainMenu);
		mGameHud->setVisible(false);
		mEditorHud->setVisible(false);

	}
	void RougeSystem::showOptionsPanel()
	{
		setCurrentMenu(mOptionsPanel);
	}
	void RougeSystem::closeOptionsPanel()
	{
		if (mEngine->getCurrentGame())
		{
			setCurrentMenu(mIngameMenu);
		}
		else
		{
			setCurrentMenu(mMainMenu);
		}
	}

	void RougeSystem::onEvent(ui::Event *e)
	{
		if (e->getType().compare("startGame") == 0)
		{
			startGame();
		}
	}

	void RougeSystem::newGame()
	{
		ui::MouseManager::getManager()->clearCurrentlyFiring();
		Game *oldGame = mEngine->getCurrentGame();
		if (oldGame != nullptr)
		{
			gfx::GfxEngine::getEngine()->getGameLayer()->clear();
		}

		if (mEngine->newGame("village"))
		{
			game::Game *game = game::Engine::getGame();
			if (game)
			{
				if (game->hasStarted())
				{
					startGame();
				}
				game->addEventListener("startGame", this);
			}
		}
	}
	void RougeSystem::startGame()
	{
		// Got to re-enable the game hud on a new game.
		mEngine->getGameHud()->setInteractive(true);

		mPlayerHand->setHandEnabled(true);

		mPausedGame = false;

		game::Game *game = game::Engine::getGame();
		if (!game->getCurrentMap())
		{
			am_log("START", "Unable to start game without a current map set onto the game!");
			return;
		}
		gfx::GfxEngine::getEngine()->getGameLayer()->addChild(game->getGameLayer());

/*		ui::StoreScreen *storeScreen = new ui::StoreScreen();
        storeScreen->show();
		storeScreen->setSize(360, 300);
		game::Item *test = game->create<game::Item>("wooden:shield");
		game::Store *store = new game::Store();
		game::Character *owner = new game::Character();
		store->setStoreOwner(owner);
		Inventory *inv = store->createStoreInventory();
        inv->addItem(test);*/

        /*test = game->create<game::Item>("wooden:sword");
        inv = store->createStoreInventory();
        inv->addItem(test);

		storeScreen->setStore(store);
		storeScreen->setBuyer(game->getMainCharacter());
		//store->addStoreInventory(inv);
		gfx::GfxEngine::getEngine()->getUILayer()->addChild(storeScreen);*/

		mPlayer = game->getMainCharacter();
		if (!mPlayer->getDialogueComp())
		{
			mPlayer->setDialogueComp(new game::DialogueComponent());
		}
		
		mPlayer->addEventListener("dialogue", this);
		game->getCurrentMap()->calcAllTileEdgeValues();
		game->getCurrentMap()->getTileRenderer()->updateAssetSprites();

		game::PlayerController *controller = new game::PlayerController();
		mPlayer->setController(controller);

		game->getCamera()->followObject(mPlayer.get());

		ui::GameHud *gameHud = game::Engine::getEngine()->getGameHud();
		if (gameHud)
		{
			gameHud->getCharacterScreen()->setCharacter(mPlayer);
			gameHud->getDialogueBox()->setTalker(mPlayer);
			gameHud->getDialogueChoices()->setTalker(mPlayer);
		}

		setCurrentMenu(nullptr);
		mGameHud->setVisible(true);

		game->onGameTick();
	}
	void RougeSystem::setCurrentMenu(ui::UIComponent *menu)
	{
		if (mCurrentMenu.get())
		{
			mCurrentMenu->setVisible(false);
		}
		mCurrentMenu = menu;
		if (mCurrentMenu.get())
		{
			if (mPlayerHand)
			{
				mPlayerHand->setHandEnabled(false);
			}
			
			mCurrentMenu->setVisible(true);
		}
		else
		{
			if (mPlayerHand)
			{
				mPlayerHand->setHandEnabled(true);
			}
		}
	}

	void RougeSystem::startEditor()
	{
		game::Game *game = new game::Game(mEngine);
		game->setEditorMode(true);
		mEngine->setCurrentGame(game);
		gfx::GfxEngine::getEngine()->getGameLayer()->addChild(game->getGameLayer());
		mEditorHud->setVisible(true);
		mEditorHud->setInteractive(true);
		mEditorHud->setGame(game);

		setCurrentMenu(nullptr);
	}

	gfx::TooltipItem *RougeSystem::getItemTooltip() const
	{
		return mItemTooltip;
	}
}
}
