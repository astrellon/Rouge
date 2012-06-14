#include "main.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	am::log::FileLogListener fileLogger("output.log");
	am::log::Logger::getMainLogger().addLogListener(&fileLogger);

	am::gfx::GfxEngine gfxEngine;
	am::base::Engine engine(&gfxEngine);

	am::ui::MouseManager mouseManager(&gfxEngine);

	am::sys::WinSystem win;
	win.setHInstance(hInstance);

	am::sys::RougeSystem *rougeSystem = am::sys::RougeSystem::createRougeSystem(
		&win, &engine, &gfxEngine, &mouseManager);
	win.setGameSystem(rougeSystem);
	
	rougeSystem->setSize(600, 400);
	rougeSystem->setPosition(50, 50);
	rougeSystem->setTitle("Hello!");

	rougeSystem->startLoop();

	rougeSystem->deinit();

	//delete rougeSystem;
	
	// Shut down!
}
