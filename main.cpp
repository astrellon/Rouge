#include "vld.h"
#include "main.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	VLDEnable();
	VLDSetReportOptions(VLD_OPT_REPORT_TO_FILE, L"memleaks.log");

	am::log::Logger mainLogger;
	am::log::Logger::setMainLogger(&mainLogger);

	am::log::FileLogListener fileLogger("output.log");
	am::log::Logger::getMainLogger()->addLogListener(&fileLogger);

	am::game::Engine *engine = new am::game::Engine();
	am::game::Engine::setEngine(engine);

	am::ui::MouseManager *mouseManager = new am::ui::MouseManager();
	am::ui::MouseManager::setManager(mouseManager);
	
	am::ui::KeyboardManager *keyboardManager = new am::ui::KeyboardManager();
	am::ui::KeyboardManager::setManager(keyboardManager);

	am::sys::WinSystem win;
	win.setHInstance(hInstance);

	bool unitTesting = false;
#ifdef TESTING
	unitTesting = true;
#endif
	am::sys::GameSystem *gameSystem;
	if (unitTesting)
	{
		am::sys::UnitTestSystem *unitTestSystem = am::sys::UnitTestSystem::createUnitTestSystem(&win, engine);
		gameSystem = unitTestSystem;
		gameSystem->setTitle("Rouge Game - Unit Tests");
	}
	else
	{
		am::sys::RougeSystem *rougeSystem = am::sys::RougeSystem::createRougeSystem(&win, engine);
		gameSystem = rougeSystem;
		gameSystem->setTitle("Rouge Game");
	}

	win.setGameSystem(gameSystem);
	
	gameSystem->setSize(1280, 700);
	gameSystem->setPosition(50, 50);
	
	gameSystem->startLoop();

	gameSystem->deinit();

	// Remove things which have event listeners before the
	// managers get deleted.
	delete engine;
	delete gameSystem;
	delete mouseManager;
	delete keyboardManager;
	GfxEngine::deinitGfxEngine();

	VLDReportLeaks();

	// Shut down!
}
