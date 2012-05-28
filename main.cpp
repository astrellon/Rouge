#include "main.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	am::log::FileLogListener fileLogger("output.log");
	am::log::Logger::getMainLogger().addLogListener(&fileLogger);

	am::base::Engine engine;
	am::gfx::GlGfxEngine gfxEngine;

	TestListener alan("Alan");
	TestListener mel("Melli");
	am::ui::EventManager manager;
	manager.addEventListener("male", &alan);
	manager.addEventListener("female", &mel);

	manager.fireEvent(am::ui::Event("male"));
	manager.fireEvent(am::ui::Event("female"));
	
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
