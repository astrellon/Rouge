#include "freeglut_system.h"

#include "../log/logger.h"

#include "game_system.h"
#ifdef _WIN32
// For windows specific functionality (creating directories, etc)
#	include <Windows.h>
#endif
#include <GL/freeglut.h>

#include <util/path_tokeniser.h>
using namespace am::util;

namespace am {
namespace sys {
namespace freeglut {

	FreeGlutSystem *FreeGlutSystem::s_freeglut_system = nullptr;

	FreeGlutSystem *FreeGlutSystem::getFreeGlutSystem()
	{
		return s_freeglut_system;
	}
	void FreeGlutSystem::setFreeGlutSystem(FreeGlutSystem *system)
	{
		s_freeglut_system = system;
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
		mGameSystem->display(dt);
	}
	void FreeGlutSystem::deinit()
	{
		mGameSystem->deinit();
	}

	void FreeGlutSystem::onMouseDown(am::ui::MouseButton mouseButton, int x, int y)
	{
		mGameSystem->onMouseDown(mouseButton, x, y);
	}
	void FreeGlutSystem::onMouseMove(am::ui::MouseButton mouseButton, int x, int y)
	{
		mGameSystem->onMouseMove(mouseButton, x, y);
	}
	void FreeGlutSystem::onMouseUp(am::ui::MouseButton mouseButton, int x, int y)
	{
		mGameSystem->onMouseUp(mouseButton, x, y);
	}
	void FreeGlutSystem::onKeyDown(int key, bool systemKey)
	{
		mGameSystem->onKeyDown(key, systemKey);
	}
	void FreeGlutSystem::onKeyUp(int key)
	{
		mGameSystem->onKeyUp(key);
	}

	bool FreeGlutSystem::isProgramRunning() const
	{
		return mProgramRunning;
	}
	void FreeGlutSystem::setProgramRunning(bool running)
	{
		mProgramRunning = running;
		if (!running)
		{
			glutExit();
		}
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

		init();

		glutReshapeFunc(onGlutReshape);
		glutDisplayFunc(onGlutDisplay);
		glutSpecialFunc(onGlutKeyboard);
		glutSpecialUpFunc(onGlutKeyboardUp);
		glutMouseFunc(onGlutMouse);
		glutMotionFunc(onGlutMouseMove);
		glutMainLoop();
		
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
		DWORD result = GetFileAttributes(folderName);
		// If it has the directory attribute and isn't an invalid file (doesn't exist or error)
		return result & FILE_ATTRIBUTE_DIRECTORY && result != INVALID_FILE_ATTRIBUTES;
	}
	bool FreeGlutSystem::isFile(const char *filename)
	{
		DWORD result = GetFileAttributes(filename);
		// If it doesn't have the directory attribute and isn't an invalid file (doesn't exist or error)
		return !(result & FILE_ATTRIBUTE_DIRECTORY) && result != INVALID_FILE_ATTRIBUTES;
	}
	bool FreeGlutSystem::createDirectory(const char *folderName)
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

	base::ReturnCode FreeGlutSystem::listDirectory(const char *folderName, ISystem::FolderEntryList &result)
	{
		if (!folderName || folderName[0] == '\0')
		{
			return NULL_PARAMETER;
		}

		WIN32_FIND_DATA files;
		string folder(folderName);
		if (folder.back() != '/' && folder.back() != '\\')
		{
			folder += '\\';
		}
		folder += '*';
		HANDLE find = FindFirstFile(folder.c_str(), &files);
		if (find == INVALID_HANDLE_VALUE)
		{
			return SUCCESS;
		}
		do
		{
			FolderEntry entry;
			entry.name = files.cFileName;
			entry.isDirectory = files.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
			result.push_back(entry);
		} while (FindNextFile(find, &files) != 0);

		return SUCCESS;
	}

	void FreeGlutSystem::setCursorHidden(bool hide)
	{
		if (hide != mHideCursor)
		{
			mHideCursor = hide;
			ShowCursor(!hide);
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
		FreeGlutSystem::getFreeGlutSystem()->display(0.1f);
	}
	void onGlutKeyboard(int key, int x, int y)
	{
		FreeGlutSystem::getFreeGlutSystem()->onKeyDown(key, false);
	}
	void onGlutKeyboardUp(int key, int x, int y)
	{
		FreeGlutSystem::getFreeGlutSystem()->onKeyUp(key);
	}
	void onGlutMouse(int button, int state, int x, int y)
	{
		ui::MouseButton amButton = ui::LEFT_BUTTON;
		if (button == GLUT_RIGHT_BUTTON)
		{
			amButton = ui::RIGHT_BUTTON;
		}
		else if (button == GLUT_MIDDLE_BUTTON)
		{
			amButton = ui::MIDDLE_BUTTON;
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
		FreeGlutSystem::getFreeGlutSystem()->onMouseMove(ui::LEFT_BUTTON, x, y);
	}
	void onGlutIdle()
	{
		FreeGlutSystem::getFreeGlutSystem()->update(0.1f);
	}


}
}
}
