#include "main.h"

// Navigation related
int oldMouseButton = 0, oldX = -1, oldY = -1;
bool mouseLeftDown = false;
bool mouseRightDown = false;

void mouseFunc(int mouseButton, int x, int y)
{
	// If we are pressing a mouse button that was different to the last
	// time we had a mouse move event, then we want to reset the oldX and oldY
	// coordinates.
	if(mouseButton != oldMouseButton)
	{
		oldX = x;
		oldY = y;
	}
	
	// If the user is pressing the left mouse button.
	if(mouseButton == 1)
	{
		int dx = x - oldX;
		int dy = y - oldY;
	}
	// If the user is pressing the right mouse button.
	else if(mouseButton == 2)
	{
		//zoom += y - oldY;
	}

	oldX = x;
	oldY = y;

	oldMouseButton = mouseButton;
}

void keyboardFunc(BOOL *keys, int key)
{
	if(!keys[key])
	{
		
	}
}

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	am::log::FileLogListener fileLogger("output.log");
	am::log::Logger::getMainLogger().addLogListener(&fileLogger);

	am_log("Test", "Hello");

	am::base::Engine engine;
	am::gfx::GlGfxEngine gfxEngine;
	

	am::sys::WinSystem win;
	win.setHInstance(hInstance);

	am::sys::GameSystem gameSystem(&win, &engine, &gfxEngine);
	win.setGameSystem(&gameSystem);
	
	gameSystem.setSize(600, 400);
	gameSystem.setPosition(50, 50);
	gameSystem.setTitle("Hello!");

	gameSystem.startLoop();

	// Shut down!
}