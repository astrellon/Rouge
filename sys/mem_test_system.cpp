#include "mem_test_system.h"

#include <gfx/gfx_engine.h>
#include <gfx/gfx_text_list.h>
#include <gfx/gfx_asset.h>
#include <gfx/gfx_text_field2.h>
using namespace am::gfx;

#include <game/game.h>
#include <game/engine.h>

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
		
		GfxEngine *gfxEngine = GfxEngine::getEngine();
		mDebugConsole->setMaxEntries(100);

		mDebugConsole->setVisible(true);

		Handle<TextField2> testText(new TextField2());
		testText->setText("Hello there <gameobj class='character'>Melli</gameobj>");
		gfxEngine->getUILayer()->addChild(testText);
		testText->setPosition(200.0f, 100.0f);

		{
			Handle<Game> game(new Game());
			Engine::getEngine()->setCurrentGame(game);
			game->addDialogue(new Dialogue("diag1", "Test text"));
			game->addDialogue(new Dialogue("diag2", "Test text 2"));

			Handle<Map> map(new Map("testMap", 4, 4));
			game->setCurrentMap(map);

			Handle<Character> mainChar(new Character());
			mainChar->setName("Melli");
			game->setMainCharacter(mainChar);
			game->registerGameObject(mainChar);
			game->addGameObject(mainChar);

			Handle<Character> npc(new Character());
			npc->setName("Townsman");
			game->registerGameObject(npc);
			game->addGameObject(npc);
		}
		{
			Handle<Game> game2(new Game());
			Engine::getEngine()->setCurrentGame(game2);
		}

	}
	
}
}
