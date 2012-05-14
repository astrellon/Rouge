#pragma once

#include <Windows.h>
#include <gl/GL.h>

#include <string>

using namespace std;

namespace am {
namespace ui {

typedef struct {									// Structure For Keyboard Stuff
	BOOL keyDown [256];								// Holds TRUE / FALSE For Each Key
} Keys;												// Keys

typedef struct {									// Contains Information Vital To Applications
	HINSTANCE		hInstance;						// Application Instance
	const char*		className;						// Application ClassName
} Application;										// Application

typedef struct {									// Window Creation Info
	Application*		application;				// Application Structure
	const char*				title;						// Window Title
	int					width;						// Width
	int					height;						// Height
	int					bitsPerPixel;				// Bits Per Pixel
	BOOL				isFullScreen;				// FullScreen?
} GL_WindowInit;									// GL_WindowInit

class WinWrap {
public:
	WinWrap();
	~WinWrap();

	int setSize(int width, int height);
	int setPosition(int x, int y);

	int getWidth() const;
	int getHeight() const;

	int getX() const;
	int getY() const;

	void setTitle(const string &title);
	string getTitle() const;

	void reshape(int width, int height);
	
	bool isRunning() const;
	void setRunning(bool running);

	int init(HINSTANCE hInstance);

protected:

	string mTitle;
	int mWidth;
	int mHeight;
	int mXpos;
	int mYpos;

};

typedef struct {									// Contains Information Vital To A Window
	Keys*				keys;						// Key Structure
	HWND				hWnd;						// Window Handle
	HDC					hDC;						// Device Context
	HGLRC				hRC;						// Rendering Context
	GL_WindowInit		init;						// Window Init
	BOOL				isVisible;					// Window Visible?
	DWORD				lastTickCount;				// Tick Counter
	WinWrap *			winWrap;
} GL_Window;										// GL_Window


BOOL ChangeScreenResolution (int width, int height, int bitsPerPixel);
BOOL CreateWindowGL (GL_Window* window);
BOOL DestroyWindowGL (GL_Window* window);
BOOL RegisterWindowClass (Application* application);
LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

}
}
