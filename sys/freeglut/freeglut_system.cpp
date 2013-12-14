#include "freeglut_system.h"

#include <log/logger.h>

#include <sys/game_system.h>
#ifdef _WIN32
// For windows specific functionality (creating directories, etc)
#	include <Windows.h>
#else
#   include <unistd.h>
#endif
#include <GL/freeglut.h>

#include <util/path_tokeniser.h>

namespace am {
namespace sys {
namespace freeglut {

	FreeGlutSystem *FreeGlutSystem::sFreeglutSystem = nullptr;

	FreeGlutSystem *FreeGlutSystem::getFreeGlutSystem()
	{
		return sFreeglutSystem;
	}
	void FreeGlutSystem::setFreeGlutSystem(FreeGlutSystem *system)
	{
		sFreeglutSystem = system;
	}

	FreeGlutSystem::FreeGlutSystem() :
		OsSystem(),
		mWidth(-1),
		mHeight(-1),
		mXpos(0),
		mYpos(0),
		mProgramRunning(false),
		mHideCursor(false),
		mFullscreen(false)
	{
	
	}
	FreeGlutSystem::~FreeGlutSystem()
	{

	}

	void FreeGlutSystem::setSize(int width, int height)
	{
		mWidth = width;
		mHeight = height;
		updatePosSize();
	}
	void FreeGlutSystem::setPosition(int x, int y)
	{
		mXpos = x;
		mYpos = y;
		updatePosSize();
	}

	int FreeGlutSystem::getWidth() const
	{
		return mWidth;
	}
	int FreeGlutSystem::getHeight() const
	{
		return mHeight;
	}

	int FreeGlutSystem::getX() const
	{
		return mXpos;
	}
	int FreeGlutSystem::getY() const
	{
		return mYpos;
	}

	void FreeGlutSystem::setTitle(const char *title)
	{
		mTitle = title;
		if (mProgramRunning)
		{
			glutSetWindowTitle(title);
		}
	}
	const char *FreeGlutSystem::getTitle() const
	{
		return mTitle.c_str();
	}

	void FreeGlutSystem::init()
	{
		try
		{
			mGameSystem->init();
		}
		catch (std::runtime_error const &e)
		{
			// TODO
			//MessageBox(mHWnd, e.what(), "Fatal startup error", MB_OK);
			
			setProgramRunning(false);
		}
	}
	void FreeGlutSystem::reshape(int width, int height)
	{
		mGameSystem->reshape(width, height);
	}
	void FreeGlutSystem::update(float dt)
	{
		mGameSystem->update(dt);
	}
	void FreeGlutSystem::display(float dt)
	{
		if (dt < 0.0f)
		{
			dt = mDeltaTime;
		}
		mGameSystem->display(dt);
		glutSwapBuffers();
	}
	void FreeGlutSystem::deinit()
	{
		mGameSystem->deinit();
	}

	void FreeGlutSystem::onMouseDown(ui::Mouse::Button mouseButton, int x, int y)
	{
		mGameSystem->onMouseDown(mouseButton, x, y);
	}
	void FreeGlutSystem::onMouseMove(ui::Mouse::Button mouseButton, int x, int y)
	{
		mGameSystem->onMouseMove(mouseButton, x, y);
	}
	void FreeGlutSystem::onMouseUp(ui::Mouse::Button mouseButton, int x, int y)
	{
		mGameSystem->onMouseUp(mouseButton, x, y);
	}
	void FreeGlutSystem::onKeyDown(ui::Keyboard::Key key)
	{
		mGameSystem->onKeyDown(key);
	}
	void FreeGlutSystem::onKeyUp(ui::Keyboard::Key key)
	{
		mGameSystem->onKeyUp(key);
	}
	void FreeGlutSystem::onKeyPress(char key)
	{
		mGameSystem->onKeyPress(key);
	}

	bool FreeGlutSystem::isProgramRunning() const
	{
		return mProgramRunning;
	}
	void FreeGlutSystem::setProgramRunning(bool running)
	{
		mProgramRunning = running;
	}

	void FreeGlutSystem::setFullscreen(bool fullscreen)
	{
		if (fullscreen)
		{
			glutFullScreen();
		}
		else
		{
			glutReshapeWindow(mWidth, mHeight);
			glutPositionWindow(0,0);
		}
		mFullscreen = fullscreen;
	}
	bool FreeGlutSystem::getFullscreen() const
	{
		return mFullscreen;
	}

	int FreeGlutSystem::startLoop(int argc, char **argv)
	{
		if (mProgramRunning)
		{
			return 1;
		}

		mProgramRunning = true;

		if (mTitle.empty())
		{
			mTitle = "AM Rouge Game";
		}
		
		if (mWidth < 1)
		{
			mWidth = 800;
		}

		if (mHeight < 1)
		{
			mHeight = 500;
		}

		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
		glutInitWindowPosition(mXpos, mYpos);
		glutInitWindowSize(mWidth, mHeight);
		glutCreateWindow(mTitle.c_str());
        setCursorHidden(true);

		init();

		glutReshapeFunc(onGlutReshape);
		glutDisplayFunc(onGlutDisplay);
		glutKeyboardFunc(onGlutKeyboard);
		glutKeyboardUpFunc(onGlutKeyboardUp);
		glutSpecialFunc(onGlutSpecialKeyboard);
		glutSpecialUpFunc(onGlutSpecialKeyboardUp);
		glutMouseFunc(onGlutMouse);
		glutMotionFunc(onGlutMouseMove);
		glutPassiveMotionFunc(onGlutMouseMove);

#ifdef _WIN32
		mTickCount = GetTickCount();
#else
        mTickCount = 0;
#endif
		
		while (mProgramRunning)
		{
#ifdef _WIN32
			DWORD tickCount = GetTickCount ();				// Get The Tick Count
#else
            long tickCount = mTickCount + 20;
#endif

			mDeltaTime = (tickCount - mTickCount) / 1000.0f;
			int diff = 25;
			diff -= tickCount - mTickCount;
			if(diff < 10)
			{
				diff = 10;
			}

			mTickCount = tickCount;

			update(mDeltaTime);
			glutPostRedisplay();

			glutMainLoopEvent();
			
#ifdef _WIN32
			Sleep(diff);
#else
            usleep(diff * 1000);
#endif
		}
        setCursorHidden(false);
		
		deinit();

		return 0;
	}

	void FreeGlutSystem::stopLoop()
	{
		if (!mProgramRunning)
		{
			return;
		}

		mProgramRunning = false;
	}

	bool FreeGlutSystem::isDirectory(const char *folderName)
	{
#ifdef _WIN32
		DWORD result = GetFileAttributes(folderName);
		// If it has the directory attribute and isn't an invalid file (doesn't exist or error)
		return result & FILE_ATTRIBUTE_DIRECTORY && result != INVALID_FILE_ATTRIBUTES;
#else
        return false;
#endif
	}
	bool FreeGlutSystem::isFile(const char *filename)
	{
#ifdef _WIN32
		DWORD result = GetFileAttributes(filename);
		// If it doesn't have the directory attribute and isn't an invalid file (doesn't exist or error)
		return !(result & FILE_ATTRIBUTE_DIRECTORY) && result != INVALID_FILE_ATTRIBUTES;
#else
        return false;
#endif
	}
	bool FreeGlutSystem::createDirectory(const char *folderName)
	{
#ifdef _WIN32
		if (folderName == nullptr || folderName[0] == '\0')
		{
			return false;
		}

		std::string path;
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
					std::string err("Error creating folder path ");
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
#else
        return false;
#endif
	}

	base::ReturnCode FreeGlutSystem::listDirectory(const char *folderName, ISystem::FolderEntryList &result)
	{
#ifdef _WIN32
		if (!folderName || folderName[0] == '\0')
		{
			return base::NULL_PARAMETER;
		}

		WIN32_FIND_DATA files;
		std::string folder(folderName);
		if (folder.back() != '/' && folder.back() != '\\')
		{
			folder += '\\';
		}
		folder += '*';
		HANDLE find = FindFirstFile(folder.c_str(), &files);
		if (find == INVALID_HANDLE_VALUE)
		{
			return base::SUCCESS;
		}
		do
		{
			FolderEntry entry;
			entry.name = files.cFileName;
			entry.isDirectory = files.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
			result.push_back(entry);
		} while (FindNextFile(find, &files) != 0);

		return base::SUCCESS;
#else
        return base::SUCCESS;
#endif
	}

	void FreeGlutSystem::setCursorHidden(bool hide)
	{
		if (hide != mHideCursor)
		{
			mHideCursor = hide;
#ifdef _WIN32
			ShowCursor(!hide);
#else
            if (hide)
            {
                am_log("CURSOR", "HIDE");
                glutSetCursor(GLUT_CURSOR_NONE);
            }
            else
            {
                am_log("CURSOR", "INHERIT");
                glutSetCursor(GLUT_CURSOR_INHERIT);
            }
#endif
			onCursorHiddenChange(hide);
		}
	}
	void FreeGlutSystem::onCursorHiddenChange(bool hide)
	{
		mGameSystem->onCursorHiddenChange(hide);
	}
	bool FreeGlutSystem::isCursorHidden() const
	{
		return mHideCursor;
	}

	void FreeGlutSystem::updatePosSize()
	{
		if (mProgramRunning)
		{
			glutPositionWindow(mXpos, mYpos);
			glutReshapeWindow(mWidth, mHeight);
		}
	}

	void onGlutReshape(int width, int height)
	{
		FreeGlutSystem::getFreeGlutSystem()->reshape(width, height);
	}
	void onGlutDisplay()
	{
		FreeGlutSystem::getFreeGlutSystem()->display(-1.0f);
	}
	void onGlutKeyboard(unsigned char key, int x, int y)
	{
		FreeGlutSystem::getFreeGlutSystem()->onKeyPress(key);
        FreeGlutSystem::getFreeGlutSystem()->onKeyDown(static_cast<ui::Keyboard::Key>(key));
	}
	void onGlutKeyboardUp(unsigned char key, int x, int y)
	{
		//FreeGlutSystem::getFreeGlutSystem()->onKeyUp(key);
        FreeGlutSystem::getFreeGlutSystem()->onKeyUp(static_cast<ui::Keyboard::Key>(key));
	}
	void onGlutSpecialKeyboard(int key, int x, int y)
	{
		FreeGlutSystem::getFreeGlutSystem()->onKeyDown(processGlutKey(key));
	}
	void onGlutSpecialKeyboardUp(int key, int x, int y)
	{
		FreeGlutSystem::getFreeGlutSystem()->onKeyUp(processGlutKey(key));
	}
	void onGlutMouse(int button, int state, int x, int y)
	{
		ui::Mouse::Button amButton = ui::Mouse::LEFT_BUTTON;
		if (button == GLUT_RIGHT_BUTTON)
		{
			amButton = ui::Mouse::RIGHT_BUTTON;
		}
		else if (button == GLUT_MIDDLE_BUTTON)
		{
			amButton = ui::Mouse::MIDDLE_BUTTON;
		}
		if (state == GLUT_UP)
		{
			FreeGlutSystem::getFreeGlutSystem()->onMouseUp(amButton, x, y);
		}
		else
		{
			FreeGlutSystem::getFreeGlutSystem()->onMouseDown(amButton, x, y);
		}
	}
	void onGlutMouseMove(int x, int y)
	{
		FreeGlutSystem::getFreeGlutSystem()->onMouseMove(ui::Mouse::LEFT_BUTTON, x, y);
	}

	ui::Keyboard::Key processGlutKey(int key)
	{
		switch (key)
		{
		case GLUT_KEY_PAGE_DOWN:		return ui::Keyboard::KEY_PAGEDOWN;
		case GLUT_KEY_PAGE_UP:			return ui::Keyboard::KEY_PAGEUP;
		case GLUT_KEY_HOME:				return ui::Keyboard::KEY_HOME;
		case GLUT_KEY_END:				return ui::Keyboard::KEY_END;
		case GLUT_KEY_LEFT:				return ui::Keyboard::KEY_LEFT_ARROW;
		case GLUT_KEY_RIGHT:			return ui::Keyboard::KEY_RIGHT_ARROW;
		case GLUT_KEY_UP:				return ui::Keyboard::KEY_UP_ARROW;
		case GLUT_KEY_DOWN:				return ui::Keyboard::KEY_DOWN_ARROW;
		case GLUT_KEY_INSERT:			return ui::Keyboard::KEY_INSERT;
		case GLUT_KEY_F1:				return ui::Keyboard::KEY_F1;
		case GLUT_KEY_F2:				return ui::Keyboard::KEY_F2;
		case GLUT_KEY_F3:				return ui::Keyboard::KEY_F3;
		case GLUT_KEY_F4:				return ui::Keyboard::KEY_F4;
		case GLUT_KEY_F5:				return ui::Keyboard::KEY_F5;
		case GLUT_KEY_F6:				return ui::Keyboard::KEY_F6;
		case GLUT_KEY_F7:				return ui::Keyboard::KEY_F7;
		case GLUT_KEY_F8:				return ui::Keyboard::KEY_F8;
		case GLUT_KEY_F9:				return ui::Keyboard::KEY_F9;
		case GLUT_KEY_F10:				return ui::Keyboard::KEY_F10;
		case GLUT_KEY_F11:				return ui::Keyboard::KEY_F11;
		case GLUT_KEY_F12:				return ui::Keyboard::KEY_F12;
		default:						return ui::Keyboard::KEY_NONE;
		}
	}

}
}
}
