#include "main.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	am::log::FileLogListener fileLogger("output.log");
	am::log::Logger::getMainLogger().addLogListener(&fileLogger);

	am::base::Engine engine;
	am::gfx::GfxEngine gfxEngine;

	am::sys::WinSystem win;
	win.setHInstance(hInstance);

	am::sys::GameSystem gameSystem(&win, &engine, &gfxEngine);
	win.setGameSystem(&gameSystem);
	
	gameSystem.setSize(600, 400);
	gameSystem.setPosition(50, 50);
	gameSystem.setTitle("Hello!");

	gameSystem.startLoop();

	gameSystem.deinit();
	// Shut down!
}
