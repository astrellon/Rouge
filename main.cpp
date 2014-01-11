#ifdef TESTING_MEM
#	define VLD_FORCE_ENABLE
#	include "vld.h"
#endif
#include "main.h"

#include <time.h>

#include <util/utils.h>

#ifdef _WIN_SYS
HINSTANCE mainHInstance;
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	mainHInstance = hInstance;
	return am::amMain(__argc, __argv);
}
#elif defined(_FREEGLUT_SYS)
int main(int argc, char **argv)
{
	return am::amMain(argc, argv);
}
#endif

namespace am {

	int amMain(int argc, char **argv)
	{
	#ifdef TESTING_MEM
		VLDEnable();
		VLDSetReportOptions(VLD_OPT_REPORT_TO_FILE, L"memleaks.log");
		//VLDSetOptions(VLD_OPT_AGGREGATE_DUPLICATES, 1000, 1000);
	#endif

		util::Utils::setRandSeed(time(nullptr));

		log::Logger mainLogger;
		log::Logger::setMainLogger(&mainLogger);

		log::FileLogListener fileLogger("output.log");
		log::Logger::getMainLogger()->addLogListener(&fileLogger);

		game::Engine *engine = new game::Engine();
		engine->retain();
		game::Engine::setEngine(engine);

		ui::MouseManager *mouseManager = new ui::MouseManager();
		mouseManager->retain();
		mouseManager->setGfxEngine(gfx::GfxEngine::getEngine());
		ui::MouseManager::setManager(mouseManager);
	
		ui::KeyboardManager *keyboardManager = new ui::KeyboardManager();
		keyboardManager->retain();
		ui::KeyboardManager::setManager(keyboardManager);

		sys::OsSystem *osSys = nullptr;
	#ifdef _WIN_SYS
	
		sys::win::WinSystem *winSys = new sys::win::WinSystem();
		winSys->setHInstance(mainHInstance);
		osSys = winSys;
	#elif defined _FREEGLUT_SYS
		sys::freeglut::FreeGlutSystem *freeGlutSys = new sys::freeglut::FreeGlutSystem();
		sys::freeglut::FreeGlutSystem::setFreeGlutSystem(freeGlutSys);
		osSys = freeGlutSys;
	#endif


		sys::GameSystem *gameSystem;
#ifdef TESTING
		{
			sys::UnitTestSystem *unitTestSystem = sys::UnitTestSystem::createUnitTestSystem(osSys, engine);
			gameSystem = unitTestSystem;
			gameSystem->setTitle("Rouge Game - Unit Tests");
		}
	#elif TESTING_MEM
		{
			sys::MemoryTestSystem *memTestSystem = sys::MemoryTestSystem::createMemoryTestSystem(osSys, engine);
			gameSystem = memTestSystem;
			gameSystem->setTitle("Rouge Game - Memory Leak Test");
		}
#else
		{
			sys::RougeSystem *rougeSystem = sys::RougeSystem::createRougeSystem(osSys, engine);
			gameSystem = rougeSystem;
			gameSystem->setTitle("Rouge Game");
		}
		#endif
		gameSystem->retain();
		osSys->setGameSystem(gameSystem);
	
		gameSystem->setSize(880, 600);
		gameSystem->setPosition(50, 50);
	
		gameSystem->startLoop(argc, argv);

		// The main game loop has finished, shut down phase.
		gameSystem->deinit();
		gameSystem->release();

		// Remove things which have event listeners before the
		// managers get deleted.
		engine->deinit();
		gfx::GfxEngine::deinitGfxEngine();
		sfx::SfxEngine::deinitSfxEngine();
		engine->release();
		util::Colour::removeAllColours();

		log::Logger::clearMainLogger();

	#ifdef TESTING_MEM
		VLDReportLeaks();
	#endif

		// Shut down!
		return 0;
	}

}
