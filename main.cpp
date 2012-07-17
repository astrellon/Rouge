#include "main.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//_CrtSetBreakAlloc(237);
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

	am::sys::RougeSystem *rougeSystem = am::sys::RougeSystem::createRougeSystem(&win, engine);
	win.setGameSystem(rougeSystem);
	
	rougeSystem->setSize(600, 400);
	rougeSystem->setPosition(50, 50);
	rougeSystem->setTitle("Hello!");

	rougeSystem->startLoop();

	rougeSystem->deinit();

	// Remove things which have event listeners before the
	// managers get deleted.
	delete engine;
	delete rougeSystem;
	delete mouseManager;
	delete keyboardManager;

	//_CrtDumpMemoryLeaks();
	
	// Shut down!
}
