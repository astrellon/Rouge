#include "mem_test_system.h"

#include <gfx/gfx_engine.h>
#include <gfx/gfx_text_list.h>
#include <gfx/gfx_asset.h>
#include <gfx/gfx_text_field2.h>

#include <game/game.h>
#include <game/engine.h>
#include <game/tile_type.h>

namespace am {
namespace sys {

	MemoryTestSystem *MemoryTestSystem::sMemorySystem = nullptr;

	MemoryTestSystem *MemoryTestSystem::createMemoryTestSystem(OsSystem *linked, game::Engine *engine)
	{
		sGameSystem = sMemorySystem = new MemoryTestSystem(linked, engine);
		return sMemorySystem;
	}
	MemoryTestSystem *MemoryTestSystem::getMemoryTestSystem()
	{
		return sMemorySystem;
	}

	MemoryTestSystem::MemoryTestSystem(OsSystem *linked, game::Engine *engine) :
		GameSystem(linked, engine)
	{

	}
	MemoryTestSystem::~MemoryTestSystem()
	{
	}

	void MemoryTestSystem::init()
	{
		GameSystem::init();
		gfx::TextStyle::loadStylesLua("data/textStyles.lua");
		
		gfx::GfxEngine *gfxEngine = gfx::GfxEngine::getEngine();
		mDebugConsole->setMaxEntries(100);

		mDebugConsole->setVisible(true);

		base::Handle<gfx::TextField2> testText(new gfx::TextField2());
		testText->setText("Hello there <gameobj class='character'>Melli</gameobj>");
		gfxEngine->getUILayer()->addChild(testText);
		testText->setPosition(200.0f, 100.0f);

		{
			game::Engine *eng = game::Engine::getEngine();

			base::Handle<game::Game> game(new game::Game());
			eng->setCurrentGame(game);
			game->addDialogue(new game::Dialogue("diag1", "Test text"));
			game->addDialogue(new game::Dialogue("diag2", "Test text 2"));

			base::Handle<game::Map> map(game->getMapLua("testMap"));
			game->setCurrentMap(map);
			//base::Handle<Map> map(new Map("testMap", 4, 4));
			//game->setCurrentMap(map);

			base::Handle<game::Character> mainChar(new game::Character());
			mainChar->setName("Melli");
			game->setMainCharacter(mainChar);
			game->registerGameObject(mainChar);
			game->addGameObjectToMap(mainChar);

			base::Handle<game::Character> npc(new game::Character());
			npc->setName("Townsman");
			game->registerGameObject(npc);
			game->addGameObjectToMap(npc);

			base::Handle<game::Item> sword(new game::Item());
			sword->setItemFullname("Sword", "Iron", "of stab");
			sword->setItemType(ItemCommon::SWORD);
			sword->getStatModifiers()->addStatModifier(Stat::MAX_DAMAGE, StatModifier(5.0f, MOD_ADD));
			sword->getStatModifiers()->addStatModifier(Stat::MIN_DAMAGE, StatModifier(3.0f, MOD_ADD));
			game->addGameObjectToMap(sword);
		}
		{
			base::Handle<game::Game> game2(new game::Game());
			Engine::getEngine()->setCurrentGame(game2);
		}

	}
	
}
}
