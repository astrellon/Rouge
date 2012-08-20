#include "unit_test_system.h"

#include <gfx/gfx_engine.h>
#include <gfx/gfx_text_list.h>

#include <tests/test_base.h>
#include <tests/test_mouse_manager.h>
#include <tests/test_map.h>
#include <tests/test_character.h>

namespace am {
namespace sys {

	UnitTestSystem *UnitTestSystem::sUnitTestSystem = NULL;

	UnitTestSystem *UnitTestSystem::createUnitTestSystem(ISystem *linked, Engine *engine)
	{
		sGameSystem = sUnitTestSystem = new UnitTestSystem(linked, engine);
		return sUnitTestSystem;
	}
	UnitTestSystem *UnitTestSystem::getUnitTestSystem()
	{
		return sUnitTestSystem;
	}

	UnitTestSystem::UnitTestSystem(ISystem *linked, Engine *engine) :
		GameSystem(linked, engine)
	{

	}
	UnitTestSystem::~UnitTestSystem()
	{
	}

	void UnitTestSystem::init()
	{
		GameSystem::init();
		
		GfxEngine *gfxEngine = GfxEngine::getEngine();
		float screenWidth = static_cast<float>(gfxEngine->getScreenWidth());
		float screenHeight = static_cast<float>(gfxEngine->getScreenHeight());

		mDebugConsole->setVisible(true);

		runSuite(am::tests::TestBase);
		runSuite(am::tests::TestMouseManager);
		runSuite(am::tests::TestMap);
		runSuite(am::tests::TestCharacter);
	}
	
	void UnitTestSystem::reshape(int width, int height)
	{
		GameSystem::reshape(width, height);

		float fwidth = static_cast<float>(width);
		float fheight = static_cast<float>(height);
	}

	void UnitTestSystem::onKeyUp(int key)
	{
		GameSystem::onKeyUp(key);
	}

}
}
