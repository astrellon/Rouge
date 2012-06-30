#include "main.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	am::log::Logger mainLogger;
	am::log::Logger::setMainLogger(&mainLogger);

	am::log::FileLogListener fileLogger("output.log");
	am::log::Logger::getMainLogger()->addLogListener(&fileLogger);

	am::game::Engine engine;
	am::game::Engine::setEngine(&engine);

	am::ui::MouseManager mouseManager;
	am::ui::MouseManager::setManager(&mouseManager);

	am::ui::KeyboardManager keyboardManager;
	am::ui::KeyboardManager::setManager(&keyboardManager);

	am::sys::WinSystem win;
	win.setHInstance(hInstance);

	am::sys::RougeSystem *rougeSystem = am::sys::RougeSystem::createRougeSystem(&win, &engine);
	win.setGameSystem(rougeSystem);
	
	rougeSystem->setSize(600, 400);
	rougeSystem->setPosition(50, 50);
	rougeSystem->setTitle("Hello!");

	rougeSystem->startLoop();

	rougeSystem->deinit();

	//delete rougeSystem;
	
	// Shut down!
}
