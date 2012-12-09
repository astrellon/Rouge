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

	MemoryTestSystem *MemoryTestSystem::sMemorySystem = NULL;

	MemoryTestSystem *MemoryTestSystem::createMemoryTestSystem(ISystem *linked, Engine *engine)
	{
		sGameSystem = sMemorySystem = new MemoryTestSystem(linked, engine);
		return sMemorySystem;
	}
	MemoryTestSystem *MemoryTestSystem::getMemoryTestSystem()
	{
		return sMemorySystem;
	}

	MemoryTestSystem::MemoryTestSystem(ISystem *linked, Engine *engine) :
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
		TileType::loadStandardTileTypesLua("data/tileTypes.lua");
		
		GfxEngine *gfxEngine = GfxEngine::getEngine();
		mDebugConsole->setMaxEntries(100);

		mDebugConsole->setVisible(true);

		Handle<TextField2> testText(new TextField2());
		testText->setText("Hello there <gameobj class='character'>Melli</gameobj>");
		gfxEngine->getUILayer()->addChild(testText);
		testText->setPosition(200.0f, 100.0f);

		{
			Engine *eng = Engine::getEngine();

			Handle<Game> game(new Game());
			eng->setCurrentGame(game);
			game->addDialogue(new Dialogue("diag1", "Test text"));
			game->addDialogue(new Dialogue("diag2", "Test text 2"));

			Handle<Map> map(game->getMapLua("testMap"));
			game->setCurrentMap(map);
			//Handle<Map> map(new Map("testMap", 4, 4));
			//game->setCurrentMap(map);

			Handle<Character> mainChar(new Character());
			mainChar->setName("Melli");
			game->setMainCharacter(mainChar);
			game->registerGameObject(mainChar);
			game->addGameObject(mainChar);

			Handle<Character> npc(new Character());
			npc->setName("Townsman");
			game->registerGameObject(npc);
			game->addGameObject(npc);

			Handle<Item> sword(new Item());
			sword->setItemFullname("Sword", "Iron", "of stab");
			sword->setItemType(ItemCommon::SWORD);
			sword->getStatModifiers().addStatModifier(Stat::MAX_DAMAGE, StatModifier(5.0f, MOD_ADD));
			sword->getStatModifiers().addStatModifier(Stat::MIN_DAMAGE, StatModifier(3.0f, MOD_ADD));
			game->addGameObject(sword);
		}
		{
			Handle<Game> game2(new Game());
			Engine::getEngine()->setCurrentGame(game2);
		}

	}
	
}
}
