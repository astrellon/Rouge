#include "mem_test_system.h"

#include <gfx/gfx_engine.h>
#include <gfx/gfx_text_list.h>
#include <gfx/gfx_asset.h>
#include <gfx/gfx_text_field2.h>
using namespace am::gfx;

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
	}
	
}
}
