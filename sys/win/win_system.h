#pragma once

#include <Windows.h>
#include <gl/GL.h>

#include <ui/mouse_common.h>
#include <ui/keyboard_common.h>

#include <string>

#include <sys/os_system.h>


namespace am {
namespace sys {
	class GameSystem;

namespace win {

	typedef struct 
	{													// Contains Information Vital To Applications
		HINSTANCE		hInstance;						// Application Instance
		const char*		className;						// Application ClassName
	} Application;										// Application

	typedef struct
	{													// Window Creation Info
		Application*		application;				// Application Structure
		const char*			title;						// Window Title
		int					width;						// Width
		int					height;						// Height
		int					bitsPerPixel;				// Bits Per Pixel
		BOOL				isFullScreen;				// FullScreen?
	} GL_WindowInit;									// GL_WindowInit

	

	class WinSystem : public am::sys::OsSystem
	{
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

		virtual void onMouseDown(ui::Mouse::Button mouseButton, int x, int y);
		virtual void onMouseMove(int x, int y);
		virtual void onMouseUp(ui::Mouse::Button mouseButton, int x, int y);
		virtual void onKeyDown(ui::Keyboard::Key key);
		virtual void onKeyUp(ui::Keyboard::Key key);
		virtual void onKeyPress(char key);
	
		virtual bool isProgramRunning() const;
		virtual void setProgramRunning(bool running);

		virtual int startLoop(int argc, char **argv);
		virtual void stopLoop();

		virtual void setCursorHidden(bool hide);
		virtual void onCursorHiddenChange(bool hidden);
		virtual bool isCursorHidden() const;

		virtual void setFullscreen(bool fullscreen);
		virtual bool getFullscreen() const;

		virtual bool isDirectory(const char *folderName);
		virtual bool isFile(const char *filename);
		virtual bool createDirectory(const char *folderName);
		virtual base::ReturnCode listDirectory(const char *folderName, ISystem::FolderEntryList &result);

		void setHInstance(HINSTANCE hInstance);
		HINSTANCE getHInstance();

		void setHWnd(HWND hWnd);
		HWND getHWnd();

	protected:

		std::string mTitle;
		int mWidth;
		int mHeight;
		int mXpos;
		int mYpos;
		bool mProgramRunning;
		bool mHideCursor;
		bool mFullscreen;

		HWND mHWnd;
		HINSTANCE mHInstance;
		
		void updatePosSize();
	};

	typedef struct
	{													// Contains Information Vital To A Window
		HWND				hWnd;						// Window base::Handle
		HDC					hDC;						// Device Context
		HGLRC				hRC;						// Rendering Context
		GL_WindowInit		init;						// Window Init
		BOOL				isVisible;					// Window Visible?
		DWORD				lastTickCount;				// Tick Counter
		WinSystem *			winSystem;
	} GL_Window;										// GL_Window


	BOOL ChangeScreenResolution (int width, int height, int bitsPerPixel);
	std::string CreateWindowGL (GL_Window* window);
	BOOL DestroyWindowGL (GL_Window* window);
	BOOL RegisterWindowClass (Application* application);
	LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	ui::Keyboard::Key processWmKey(int key);

}
}
}
