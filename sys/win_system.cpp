#include "win_system.h"

#include "../log/logger.h"

#include "game_system.h"

#include <util/path_tokeniser.h>
using namespace am::util;

namespace am {
namespace sys {

	WinSystem::WinSystem() :
		mHWnd(nullptr),
		mHInstance(nullptr),
		mWidth(-1),
		mHeight(-1),
		mXpos(0),
		mYpos(0),
		mRunning(false),
		mHideCursor(false),
		mGameSystem(nullptr),
		mFullscreen(false)
	{
	
	}
	WinSystem::~WinSystem()
	{

	}

	void WinSystem::setSize(int width, int height)
	{
		mWidth = width;
		mHeight = height;
		updatePosSize();
	}
	void WinSystem::setPosition(int x, int y)
	{
		mXpos = x;
		mYpos = y;
		updatePosSize();
	}

	int WinSystem::getWidth() const
	{
		return mWidth;
	}
	int WinSystem::getHeight() const
	{
		return mHeight;
	}

	int WinSystem::getX() const
	{
		return mXpos;
	}
	int WinSystem::getY() const
	{
		return mYpos;
	}

	void WinSystem::setTitle(const char *title)
	{
		mTitle = title;
		if (mHWnd != nullptr)
		{
			SetWindowText(mHWnd, mTitle.c_str());
		}
	}
	const char *WinSystem::getTitle() const
	{
		return mTitle.c_str();
	}

	void WinSystem::init()
	{
		try
		{
			mGameSystem->init();
		}
		catch (std::runtime_error const &e)
		{
			MessageBox(mHWnd, e.what(), "Fatal startup error", MB_OK);
			setProgramRunning(false);
		}
	}
	void WinSystem::reshape(int width, int height)
	{
		mGameSystem->reshape(width, height);
	}
	void WinSystem::update(float dt)
	{
		mGameSystem->update(dt);
	}
	void WinSystem::display(float dt)
	{
		mGameSystem->display(dt);
	}
	void WinSystem::deinit()
	{
		mGameSystem->deinit();
	}

	void WinSystem::onMouseDown(am::ui::MouseButton mouseButton, int x, int y)
	{
		mGameSystem->onMouseDown(mouseButton, x, y);
	}
	void WinSystem::onMouseMove(am::ui::MouseButton mouseButton, int x, int y)
	{
		mGameSystem->onMouseMove(mouseButton, x, y);
	}
	void WinSystem::onMouseUp(am::ui::MouseButton mouseButton, int x, int y)
	{
		mGameSystem->onMouseUp(mouseButton, x, y);
	}
	void WinSystem::onKeyDown(int key, bool systemKey)
	{
		mGameSystem->onKeyDown(key, systemKey);
	}
	void WinSystem::onKeyUp(int key)
	{
		mGameSystem->onKeyUp(key);
	}

	bool WinSystem::isProgramRunning() const
	{
		return mProgramRunning;
	}
	void WinSystem::setProgramRunning(bool running)
	{
		PostMessage (mHWnd, WM_QUIT, 0, 0);
		mProgramRunning = running;
	}

	void WinSystem::setFullscreen(bool fullscreen)
	{
		if (fullscreen)
		{
			SetWindowLongPtr(mHWnd, GWL_STYLE, 
				WS_SYSMENU | WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_VISIBLE);
			MoveWindow(mHWnd, 0, 0, mWidth, mHeight, TRUE);
		}
		else
		{
			RECT rect;
			rect.left = 0;
			rect.top = 0;
			rect.right = mWidth;
			rect.bottom = mHeight;
			SetWindowLongPtr(mHWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
			MoveWindow(mHWnd, 0, 0, rect.right-rect.left, rect.bottom-rect.top, TRUE);
		}
		mFullscreen = fullscreen;
	}
	bool WinSystem::getFullscreen() const
	{
		return mFullscreen;
	}

	void WinSystem::setHWnd(HWND hWnd)
	{
		mHWnd = hWnd;
	}
	HWND WinSystem::getHWnd()
	{
		return mHWnd;
	}

	void WinSystem::setHInstance(HINSTANCE hInstance)
	{
		mHInstance = hInstance;
	}
	HINSTANCE WinSystem::getHInstance()
	{
		return mHInstance;
	}

	void WinSystem::setGameSystem(GameSystem *system)
	{
		mGameSystem = system;
	}
	GameSystem *WinSystem::getGameSystem()
	{
		return mGameSystem;
	}

	bool WinSystem::isRunning() const
	{
		return mRunning;
	}
	int WinSystem::startLoop()
	{
		if (mRunning)
		{
			return 1;
		}

		Application			application;									// Application Structure
		GL_Window			window;											// Window Structure
		//Keys				keys;											// Key Structure
		BOOL				isMessagePumpActive;							// Message	Pump Active?
		MSG					msg;											// Window Message Structure
		DWORD				tickCount;										// Used For The Tick Counter

		// Fill Out Application Data
		application.className = "OpenGL";
		application.hInstance = mHInstance;

		// Fill Out Window
		//ZeroMemory (&window, sizeof (GL_Window));
		//window.keys	= &keys;
		window.init.application = &application;
		if (mTitle.empty())
		{
			mTitle = "AM Rouge Game";
		}
		window.init.title = mTitle.c_str();
		
		if (mWidth < 1)
		{
			mWidth = 800;
		}
		window.init.width = mWidth;
		if (mHeight < 1)
		{
			mHeight = 500;
		}
		window.init.height = mHeight;

		window.init.bitsPerPixel = 24;
		window.init.isFullScreen = FALSE;

		window.winSystem = this;

		// Register A Class For Our Window To Use
		if (RegisterWindowClass (&application) == FALSE)
		{
			// Failure
			MessageBox (HWND_DESKTOP, "Error Registering Window Class!", "Error", MB_OK | MB_ICONEXCLAMATION);
			return -1;
		}

		string createWindowResult = CreateWindowGL(&window);
		if (createWindowResult.size() > 0)
		{
			// Failure
			am_log("WIN", createWindowResult);
			MessageBox (HWND_DESKTOP, createWindowResult.c_str(), "Error", MB_OK | MB_ICONEXCLAMATION);
			return -1;
		}

		window.winSystem->init();

		while (mProgramRunning)											// Loop Until WM_QUIT Is Received
		{
			//ShowCursor(false);
			isMessagePumpActive = TRUE;								// Set isMessagePumpActive To TRUE
			while (isMessagePumpActive == TRUE)						// While The Message Pump Is Active
			{
				// Success Creating Window.  Check For Window Messages
				if (PeekMessage (&msg, window.hWnd, 0, 0, PM_REMOVE) != 0)
				{
					// Check For WM_QUIT Message
					if (msg.message != WM_QUIT)						// Is The Message A WM_QUIT Message?
					{
						TranslateMessage(&msg);
						DispatchMessage (&msg);						// If Not, Dispatch The Message
					}
					else											// Otherwise (If Message Is WM_QUIT)
					{
						isMessagePumpActive = FALSE;				// Terminate The Message Pump
					}
				}
				else												// If There Are No Messages
				{
					int diff = 25;
					if (window.isVisible == FALSE)					// If Window Is Not Visible
					{
						diff = 100;
						WaitMessage ();								// Application Is Minimized Wait For A Message
					}
					else											// If Window Is Visible
					{
						// Process Application Loop
						tickCount = GetTickCount ();				// Get The Tick Count

						float dt = (tickCount - window.lastTickCount) / 1000.0f;
						window.winSystem->update(dt);

						//int diff = tickCount - window.lastTickCount;
						diff -= tickCount - window.lastTickCount;
						if(diff < 10)
						{
							diff = 10;
						}

						window.lastTickCount = tickCount;			// Set Last Count To Current Count
						
						window.winSystem->display(dt);		// Draw Our Scene

						SwapBuffers (window.hDC);					// Swap Buffers (Double Buffering)
					}

					Sleep(diff);
				}
			}

			// Application Is Finished
			window.winSystem->deinit();											// User Defined DeInitialization

			DestroyWindowGL (&window);									// Destroy The Active Window

			UnregisterClass (application.className, application.hInstance);		// UnRegister Window Class
		}
		return 0;
	}

	void WinSystem::stopLoop()
	{
		if (!mRunning)
		{
			return;
		}

		mProgramRunning = false;
		mRunning = false;
	}

	bool WinSystem::isDirectory(const char *folderName)
	{
		DWORD result = GetFileAttributes(folderName);
		// If it has the directory attribute and isn't an invalid file (doesn't exist or error)
		return result & FILE_ATTRIBUTE_DIRECTORY && result != INVALID_FILE_ATTRIBUTES;
	}
	bool WinSystem::isFile(const char *filename)
	{
		DWORD result = GetFileAttributes(filename);
		// If it doesn't have the directory attribute and isn't an invalid file (doesn't exist or error)
		return !(result & FILE_ATTRIBUTE_DIRECTORY) && result != INVALID_FILE_ATTRIBUTES;
	}
	bool WinSystem::createDirectory(const char *folderName)
	{
		if (folderName == nullptr || folderName[0] == '\0')
		{
			return false;
		}

		string path;
		PathTokeniser tokeniser(folderName);
		const char *token = tokeniser.nextToken();
		if (!token)
		{
			return false;
		}
		
		// Don't support creating network folders at this time.
		if (strcmp(token, "\\\\") == 0)
		{
			return false;
		}
		path = token;
		
		while (token)
		{
			bool result = CreateDirectory(path.c_str(), nullptr);
			if (!result)
			{
				DWORD error = GetLastError();
				// If the folder already exists then we don't care.
				if (error != ERROR_ALREADY_EXISTS)
				{
					string err("Error creating folder path ");
					err += path;
					am_log("PATH", err);
					return false;
				}
			}
			token = tokeniser.nextToken();
			if (token)
			{
				path += '\\';
				path += token;
			}
		}
		return true;
	}

	void WinSystem::setCursorHidden(bool hide)
	{
		if (hide != mHideCursor)
		{
			mHideCursor = hide;
			ShowCursor(!hide);
			onCursorHiddenChange(hide);
		}
	}
	void WinSystem::onCursorHiddenChange(bool hide)
	{
		mGameSystem->onCursorHiddenChange(hide);
	}
	bool WinSystem::isCursorHidden() const
	{
		return mHideCursor;
	}

	void WinSystem::updatePosSize()
	{
		if (mHWnd == nullptr)
		{
			return;
		}

		SetWindowPos(mHWnd, nullptr,
			mXpos, mYpos,
			mWidth, mHeight,
			SWP_SHOWWINDOW);
	}


	BOOL ChangeScreenResolution (int width, int height, int bitsPerPixel)	// Change The Screen Resolution
	{
		DEVMODE dmScreenSettings;											// Device Mode
		ZeroMemory (&dmScreenSettings, sizeof (DEVMODE));					// Make Sure Memory Is Cleared
		dmScreenSettings.dmSize				= sizeof (DEVMODE);				// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth		= width;						// Select Screen Width
		dmScreenSettings.dmPelsHeight		= height;						// Select Screen Height
		dmScreenSettings.dmBitsPerPel		= bitsPerPixel;					// Select Bits Per Pixel
		dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			return FALSE;													// Display Change Failed, Return False
		}
		return TRUE;														// Display Change Was Successful, Return True
	}

	string CreateWindowGL (GL_Window* window)									// This Code Creates Our OpenGL Window
	{
		DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// Define Our Window Style
		DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// Define The Window's Extended Style

		PIXELFORMATDESCRIPTOR pfd =											// pfd Tells Windows How We Want Things To Be
		{
			sizeof (PIXELFORMATDESCRIPTOR),									// Size Of This Pixel Format Descriptor
			1,																// Version Number
			PFD_DRAW_TO_WINDOW |											// Format Must Support Window
			PFD_SUPPORT_OPENGL |											// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,												// Must Support Double Buffering
			PFD_TYPE_RGBA,													// Request An RGBA Format
			window->init.bitsPerPixel,										// Select Our Color Depth
			0, 0, 0, 0, 0, 0,												// Color Bits Ignored
			0,																// No Alpha Buffer
			0,																// Shift Bit Ignored
			0,																// No Accumulation Buffer
			0, 0, 0, 0,														// Accumulation Bits Ignored
			16,																// 16Bit Z-Buffer (Depth Buffer)  
			0,																// No Stencil Buffer
			0,																// No Auxiliary Buffer
			PFD_MAIN_PLANE,													// Main Drawing Layer
			0,																// Reserved
			0, 0, 0															// Layer Masks Ignored
		};

		RECT windowRect = {0, 0, window->init.width, window->init.height};	// Define Our Window Coordinates

		GLuint PixelFormat;													// Will Hold The Selected Pixel Format

		if (window->init.isFullScreen == TRUE)								// Fullscreen Requested, Try Changing Video Modes
		{
			if (ChangeScreenResolution (window->init.width, window->init.height, window->init.bitsPerPixel) == FALSE)
			{
				// Fullscreen Mode Failed.  Run In Windowed Mode Instead
				MessageBox (HWND_DESKTOP, "Mode Switch Failed.\nRunning In Windowed Mode.", "Error", MB_OK | MB_ICONEXCLAMATION);
				window->init.isFullScreen = FALSE;							// Set isFullscreen To False (Windowed Mode)
			}
			else															// Otherwise (If Fullscreen Mode Was Successful)
			{
				windowStyle = WS_POPUP;										// Set The WindowStyle To WS_POPUP (Popup Window)
				windowExtendedStyle |= WS_EX_TOPMOST;						// Set The Extended Window Style To WS_EX_TOPMOST
			}																// (Top Window Covering Everything Else)
		}
		else																// If Fullscreen Was Not Selected
		{
			// Adjust Window, Account For Window Borders
			AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);
		}
		// Create The OpenGL Window
		window->hWnd = CreateWindowEx (
			windowExtendedStyle,					// Extended Style
			window->init.application->className,	// Class Name
			window->init.title,					// Window Title
			windowStyle,							// Window Style
			0, 0,								// Window X,Y Position
			windowRect.right - windowRect.left,	// Window Width
			windowRect.bottom - windowRect.top,	// Window Height
			HWND_DESKTOP,						// Desktop Is Window's Parent
			0,									// No Menu
			window->init.application->hInstance, // Pass The Window Instance
			window);

		if (window->hWnd == 0)												// Was Window Creation A Success?
		{
			return "Unable to create OpenGL Window";
		}

		window->winSystem->setHWnd(window->hWnd);

		window->hDC = GetDC(window->hWnd);									// Grab A Device Context For This Window
		if (window->hDC == 0)												// Did We Get A Device Context?
		{
			// Failed
			DestroyWindow (window->hWnd);									// Destroy The Window
			window->hWnd = 0;												// Zero The Window Handle
			return "Unable to create device context for OpenGL Window";
		}

		PixelFormat = ChoosePixelFormat (window->hDC, &pfd);				// Find A Compatible Pixel Format
		if (PixelFormat == 0)												// Did We Find A Compatible Format?
		{
			// Failed
			ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
			window->hDC = 0;												// Zero The Device Context
			DestroyWindow (window->hWnd);									// Destroy The Window
			window->hWnd = 0;												// Zero The Window Handle
			return "Unable to find compatible pixel format for OpenGL Window";
		}

		if (SetPixelFormat (window->hDC, PixelFormat, &pfd) == FALSE)		// Try To Set The Pixel Format
		{
			// Failed
			ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
			window->hDC = 0;												// Zero The Device Context
			DestroyWindow (window->hWnd);									// Destroy The Window
			window->hWnd = 0;												// Zero The Window Handle
			return "Unable to set pixel format for OpenGL Window";
		}

		window->hRC = wglCreateContext (window->hDC);						// Try To Get A Rendering Context
		if (window->hRC == 0)												// Did We Get A Rendering Context?
		{
			// Failed
			ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
			window->hDC = 0;												// Zero The Device Context
			DestroyWindow (window->hWnd);									// Destroy The Window
			window->hWnd = 0;												// Zero The Window Handle
			return "Unable to get a render context for OpenGL Window\nYou may need to reset your computer or update your graphics drivers.";
		}

		// Make The Rendering Context Our Current Rendering Context
		if (wglMakeCurrent (window->hDC, window->hRC) == FALSE)
		{
			// Failed
			wglDeleteContext (window->hRC);									// Delete The Rendering Context
			window->hRC = 0;												// Zero The Rendering Context
			ReleaseDC (window->hWnd, window->hDC);							// Release Our Device Context
			window->hDC = 0;												// Zero The Device Context
			DestroyWindow (window->hWnd);									// Destroy The Window
			window->hWnd = 0;												// Zero The Window Handle
			return "Unable to use OpenGL render context";
		}

		ShowWindow (window->hWnd, SW_NORMAL);								// Make The Window Visible
		window->isVisible = TRUE;											// Set isVisible To True

		window->winSystem->reshape(window->init.width, window->init.height);					// Reshape Our GL Window

		window->lastTickCount = GetTickCount ();							// Get Tick Count

		return "";
																			// Initialization Will Be Done In WM_CREATE
	}

	BOOL DestroyWindowGL (GL_Window* window)								// Destroy The OpenGL Window & Release Resources
	{
		if (window->hWnd != 0)												// Does The Window Have A Handle?
		{	
			if (window->hDC != 0)											// Does The Window Have A Device Context?
			{
				wglMakeCurrent (window->hDC, 0);							// Set The Current Active Rendering Context To Zero
				if (window->hRC != 0)										// Does The Window Have A Rendering Context?
				{
					wglDeleteContext (window->hRC);							// Release The Rendering Context
					window->hRC = 0;										// Zero The Rendering Context

				}
				ReleaseDC (window->hWnd, window->hDC);						// Release The Device Context
				window->hDC = 0;											// Zero The Device Context
			}
			DestroyWindow (window->hWnd);									// Destroy The Window
			window->hWnd = 0;												// Zero The Window Handle
		}

		if (window->init.isFullScreen)										// Is Window In Fullscreen Mode
		{
			ChangeDisplaySettings (nullptr,0);									// Switch Back To Desktop Resolution
			ShowCursor (TRUE);												// Show The Cursor
		}	
		return TRUE;														// Return True
	}

	// Process Window Message Callbacks
	LRESULT CALLBACK WindowProc (HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Get The Window Context
		GL_Window* window = (GL_Window*)(GetWindowLong (hWnd, GWL_USERDATA));

		switch (uMsg)														// Evaluate Window Message
		{
			case WM_SYSCOMMAND:												// Intercept System Commands
			{
				switch (wParam)												// Check System Calls
				{
					case SC_SCREENSAVE:										// Screensaver Trying To Start?
					case SC_MONITORPOWER:									// Monitor Trying To Enter Powersave?
					return 0;												// Prevent From Happening
				}
				break;														// Exit
			}
			return 0;														// Return

			case WM_CREATE:													// Window Creation
			{
				CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);			// Store Window Structure Pointer
				window = (GL_Window*)(creation->lpCreateParams);
				SetWindowLong (hWnd, GWL_USERDATA, (LONG)(window));
			}
			return 0;														// Return

			case WM_CLOSE:													// Closing The Window
				//terminateApplication(window);								// Terminate The Application
				window->winSystem->setProgramRunning(false);
			return 0;														// Return

			case WM_SIZE:													// Size Action Has Taken Place
				switch (wParam)												// Evaluate Size Action
				{
					case SIZE_MINIMIZED:									// Was Window Minimized?
						window->isVisible = FALSE;							// Set isVisible To False
					return 0;												// Return

					case SIZE_MAXIMIZED:									// Was Window Maximized?
						window->isVisible = TRUE;							// Set isVisible To True
						window->winSystem->reshape(LOWORD(lParam), HIWORD(lParam));
					return 0;												// Return

					case SIZE_RESTORED:										// Was Window Restored?
						window->isVisible = TRUE;							// Set isVisible To True
						window->winSystem->reshape(LOWORD(lParam), HIWORD(lParam));
					return 0;												// Return
				}
			break;

			case WM_MOUSEMOVE:

				am::ui::MouseButton button;
				switch(LOWORD(wParam))
				{
				default:
				case MK_LBUTTON:
					button = am::ui::LEFT_BUTTON;
					break;
				case MK_RBUTTON:
					button = am::ui::RIGHT_BUTTON;
					break;
				case MK_MBUTTON:
					button = am::ui::MIDDLE_BUTTON;
					break;
				}
				window->winSystem->onMouseMove(button, LOWORD(lParam), HIWORD(lParam));
			break;

			case WM_LBUTTONDOWN:
				window->winSystem->onMouseDown(am::ui::LEFT_BUTTON, LOWORD(lParam), HIWORD(lParam));
			break;
			case WM_LBUTTONUP:
				window->winSystem->onMouseUp(am::ui::LEFT_BUTTON, LOWORD(lParam), HIWORD(lParam));
			break;

			case WM_RBUTTONDOWN:
				window->winSystem->onMouseDown(am::ui::RIGHT_BUTTON, LOWORD(lParam), HIWORD(lParam));
			break;
			case WM_RBUTTONUP:
				window->winSystem->onMouseUp(am::ui::RIGHT_BUTTON, LOWORD(lParam), HIWORD(lParam));
			break;

			case WM_MBUTTONDOWN:
				window->winSystem->onMouseDown(am::ui::MIDDLE_BUTTON, LOWORD(lParam), HIWORD(lParam));
			break;
			case WM_MBUTTONUP:
				window->winSystem->onMouseUp(am::ui::MIDDLE_BUTTON, LOWORD(lParam), HIWORD(lParam));
			break;

			case WM_SETCURSOR:
				window->winSystem->setCursorHidden(HTCLIENT == LOWORD(lParam));
			break;

			case WM_KEYDOWN:												// Update Keyboard Buffers For Keys Pressed
				if ((wParam >= 0) && (wParam <= 255))						// Is Key (wParam) In A Valid Range?
				{
					window->winSystem->onKeyDown(wParam, true);
					return 0;												// Return
				}
			break;															// Break

			case WM_CHAR:
				if (wParam >= 0 && wParam <= 255)
				{
					window->winSystem->onKeyDown(wParam, false);
					return 0;												// Return
				}
			break;

			case WM_KEYUP:													// Update Keyboard Buffers For Keys Released
				if ((wParam >= 0) && (wParam <= 255))						// Is Key (wParam) In A Valid Range?
				{
					window->winSystem->onKeyUp(wParam);
					return 0;												// Return
				}
			break;															// Break
															// Break
		}

		return DefWindowProc (hWnd, uMsg, wParam, lParam);					// Pass Unhandled Messages To DefWindowProc
	}

	BOOL RegisterWindowClass (Application* application)						// Register A Window Class For This Application.
	{																		// TRUE If Successful
		// Register A Window Class
		WNDCLASSEX windowClass;												// Window Class
		ZeroMemory (&windowClass, sizeof (WNDCLASSEX));						// Make Sure Memory Is Cleared
		windowClass.cbSize			= sizeof (WNDCLASSEX);					// Size Of The windowClass Structure
		windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraws The Window For Any Movement / Resizing
		windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				// WindowProc Handles Messages
		windowClass.hInstance		= application->hInstance;				// Set The Instance
		windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			// Class Background Brush Color
		windowClass.hCursor			= LoadCursor(nullptr, IDC_ARROW);			// Load The Arrow Pointer
		windowClass.lpszClassName	= application->className;				// Sets The Applications Classname
		if (RegisterClassEx (&windowClass) == 0)							// Did Registering The Class Fail?
		{
			// NOTE: Failure, Should Never Happen
			MessageBox (HWND_DESKTOP, "RegisterClassEx Failed!", "Error", MB_OK | MB_ICONEXCLAMATION);
			return FALSE;													// Return False (Failure)
		}
		return TRUE;														// Return True (Success)
	}


}
}
