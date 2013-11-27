#include "mem_test_system.h"

#include <gfx/gfx_engine.h>
#include <gfx/gfx_text_list.h>
#include <gfx/gfx_asset.h>
#include <gfx/gfx_text_field2.h>
using namespace am::gfx;

#include <game/game.h>
#include <game/engine.h>
#include <game/tile_type.h>

namespace am {
namespace sys {

	MemoryTestSystem *MemoryTestSystem::sMemorySystem = nullptr;

	MemoryTestSystem *MemoryTestSystem::createMemoryTestSystem(OsSystem *linked, Engine *engine)
	{
		sGameSystem = sMemorySystem = new MemoryTestSystem(linked, engine);
		return sMemorySystem;
	}
	MemoryTestSystem *MemoryTestSystem::getMemoryTestSystem()
	{
		return sMemorySystem;
	}

	MemoryTestSystem::MemoryTestSystem(OsSystem *linked, Engine *engine) :
		GameSystem(linked, engine)
	{

	}
	MemoryTestSystem::~MemoryTestSystem()
	{
	}

	void MemoryTestSystem::init()
	{
		GameSystem::init();
		TextStyle::loadStylesLua("data/textStyles.lua");
		
		GfxEngine *gfxEngine = GfxEngine::getEngine();
		mDebugConsole->setMaxEntries(100);

		mDebugConsole->setVisible(true);

		am::base::Handle<TextField2> testText(new TextField2());
		testText->setText("Hello there <gameobj class='character'>Melli</gameobj>");
		gfxEngine->getUILayer()->addChild(testText);
		testText->setPosition(200.0f, 100.0f);

		{
			Engine *eng = Engine::getEngine();

			am::base::Handle<Game> game(new Game());
			eng->setCurrentGame(game);
			game->addDialogue(new Dialogue("diag1", "Test text"));
			game->addDialogue(new Dialogue("diag2", "Test text 2"));

			am::base::Handle<Map> map(game->getMapLua("testMap"));
			game->setCurrentMap(map);
			//am::base::Handle<Map> map(new Map("testMap", 4, 4));
			//game->setCurrentMap(map);

			am::base::Handle<Character> mainChar(new Character());
			mainChar->setName("Melli");
			game->setMainCharacter(mainChar);
			game->registerGameObject(mainChar);
			game->addGameObjectToMap(mainChar);

			am::base::Handle<Character> npc(new Character());
			npc->setName("Townsman");
			game->registerGameObject(npc);
			game->addGameObjectToMap(npc);

			am::base::Handle<Item> sword(new Item());
			sword->setItemFullname("Sword", "Iron", "of stab");
			sword->setItemType(ItemCommon::SWORD);
			sword->getStatModifiers()->addStatModifier(Stat::MAX_DAMAGE, StatModifier(5.0f, MOD_ADD));
			sword->getStatModifiers()->addStatModifier(Stat::MIN_DAMAGE, StatModifier(3.0f, MOD_ADD));
			game->addGameObjectToMap(sword);
		}
		{
			am::base::Handle<Game> game2(new Game());
			Engine::getEngine()->setCurrentGame(game2);
		}

	}
	
}
}
