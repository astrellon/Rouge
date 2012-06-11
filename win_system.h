#pragma once

#include <Windows.h>
#include <gl/GL.h>

#include "mouse_common.h"

#include <string>

#include "isystem.h"

using namespace std;

namespace am {
namespace sys {

	typedef struct {									// Structure For Keyboard Stuff
		bool keyDown [256];								// Holds TRUE / FALSE For Each Key
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

	class GameSystem;

	class WinSystem : public ISystem {
	public:
		WinSystem();
		~WinSystem();

		virtual void setSize(int width, int height);
		virtual void setPosition(int x, int y);

		virtual int getWidth() const;
		virtual int getHeight() const;

		virtual int getX() const;
		virtual int getY() const;

		virtual void setTitle(const char *title);
		virtual const char *getTitle() const;

		virtual void init();
		virtual void reshape(int width, int height);
		virtual void update(float dt);
		virtual void display(float dt);
		virtual void deinit();

		virtual void onMouseDown(am::ui::MouseButton mouseButton, int x, int y);
		virtual void onMouseMove(am::ui::MouseButton mouseButton, int x, int y);
		virtual void onMouseUp(am::ui::MouseButton mouseButton, int x, int y);
		virtual void onKeyDown(const bool *keys, int key);
		virtual void onKeyUp(const bool *keys, int key);
	
		virtual bool isProgramRunning() const;
		virtual void setProgramRunning(bool running);

		virtual bool isRunning() const;
		virtual int startLoop();
		virtual void stopLoop();

		virtual void setCursorHidden(bool hide);
		virtual void onCursorHiddenChange(bool hidden);
		virtual bool isCursorHidden() const;

		virtual void setFullscreen(bool fullscreen);
		virtual bool getFullscreen() const;

		void setHInstance(HINSTANCE hInstance);
		HINSTANCE getHInstance();

		void setHWnd(HWND hWnd);
		HWND getHWnd();

		void setGameSystem(GameSystem *game);
		GameSystem *getGameSystem();

	protected:

		string mTitle;
		int mWidth;
		int mHeight;
		int mXpos;
		int mYpos;
		bool mProgramRunning;
		bool mRunning;
		bool mHideCursor;
		bool mFullscreen;

		HWND mHWnd;
		HINSTANCE mHInstance;
		GameSystem *mGameSystem;
	
		bool *mKeysDown;

		void updatePosSize();
	};

	typedef struct {									// Contains Information Vital To A Window
		Keys*				keys;						// Key Structure
		HWND				hWnd;						// Window Handle
		HDC					hDC;						// Device Context
		HGLRC				hRC;						// Rendering Context
		GL_WindowInit		init;						// Window Init
		BOOL				isVisible;					// Window Visible?
		DWORD				lastTickCount;				// Tick Counter
		WinSystem *			winSystem;
	} GL_Window;										// GL_Window


	BOOL ChangeScreenResolution (int width, int height, int bitsPerPixel);
	BOOL CreateWindowGL (GL_Window* window);
	BOOL DestroyWindowGL (GL_Window* window);
	BOOL RegisterWindowClass (Application* application);
	LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

}
}
