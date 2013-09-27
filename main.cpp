#ifdef TESTING_MEM
#	define VLD_FORCE_ENABLE
#	include "vld.h"
#endif
#include "main.h"

#include <time.h>

#include <util/utils.h>

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#ifdef TESTING_MEM
	VLDEnable();
	VLDSetReportOptions(VLD_OPT_REPORT_TO_FILE, L"memleaks.log");
	//VLDSetOptions(VLD_OPT_AGGREGATE_DUPLICATES, 1000, 1000);
#endif

	am::util::Utils::setRandSeed(time(nullptr));

	am::log::Logger mainLogger;
	am::log::Logger::setMainLogger(&mainLogger);

	am::log::FileLogListener fileLogger("output.log");
	am::log::Logger::getMainLogger()->addLogListener(&fileLogger);

	am::game::Engine *engine = new am::game::Engine();
	engine->retain();
	am::game::Engine::setEngine(engine);

	am::ui::MouseManager *mouseManager = new am::ui::MouseManager();
	mouseManager->retain();
	am::ui::MouseManager::setManager(mouseManager);
	
	am::ui::KeyboardManager *keyboardManager = new am::ui::KeyboardManager();
	keyboardManager->retain();
	am::ui::KeyboardManager::setManager(keyboardManager);

	am::sys::OsSystem *osSys = nullptr;
#ifdef WIN32
	am::sys::WinSystem *win = new am::sys::WinSystem();
	win->setHInstance(hInstance);
	osSys = win;
#endif

	int testing = 0;
#ifdef TESTING
	testing = 1;
#elif TESTING_MEM
	testing = 2;
#endif

	am::sys::GameSystem *gameSystem;
	if (testing == 1)
	{
		am::sys::UnitTestSystem *unitTestSystem = am::sys::UnitTestSystem::createUnitTestSystem(osSys, engine);
		gameSystem = unitTestSystem;
		gameSystem->setTitle("Rouge Game - Unit Tests");
	}
	else if (testing == 2)
	{
		am::sys::MemoryTestSystem *memTestSystem = am::sys::MemoryTestSystem::createMemoryTestSystem(osSys, engine);
		gameSystem = memTestSystem;
		gameSystem->setTitle("Rouge Game - Memory Leak Test");
	}
	else
	{
		am::sys::RougeSystem *rougeSystem = am::sys::RougeSystem::createRougeSystem(osSys, engine);
		gameSystem = rougeSystem;
		gameSystem->setTitle("Rouge Game");
	}
	gameSystem->retain();
	osSys->setGameSystem(gameSystem);
	
	gameSystem->setSize(880, 700);
	gameSystem->setPosition(50, 50);
	
	gameSystem->startLoop();

	// The main game loop has finished, shut down phase.
	gameSystem->deinit();
	gameSystem->release();

	// Remove things which have event listeners before the
	// managers get deleted.
	engine->deinit();
	am::gfx::GfxEngine::deinitGfxEngine();
	am::sfx::SfxEngine::deinitSfxEngine();
	engine->release();
	am::util::Colour::removeAllColours();

	am::log::Logger::clearMainLogger();

#ifdef TESTING_MEM
	VLDReportLeaks();
#endif

	// Shut down!
}
