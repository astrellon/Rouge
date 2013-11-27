#pragma once

#include <ui/mouse_common.h>
#include <ui/keyboard_common.h>

#include <string>

#include <sys/os_system.h>

using namespace std;

namespace am {
namespace sys {
	class GameSystem;

namespace freeglut {
	
	class FreeGlutSystem : public am::sys::OsSystem {
	public:
		FreeGlutSystem();
		~FreeGlutSystem();

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

		virtual void onMouseDown(am::ui::Mouse::Button mouseButton, int x, int y);
		virtual void onMouseMove(am::ui::Mouse::Button mouseButton, int x, int y);
		virtual void onMouseUp(am::ui::Mouse::Button mouseButton, int x, int y);
		virtual void onKeyDown(am::ui::Keyboard::Key key);
		virtual void onKeyUp(am::ui::Keyboard::Key key);
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

		static FreeGlutSystem *getFreeGlutSystem();
		static void setFreeGlutSystem(FreeGlutSystem *system);

	protected:

		string mTitle;
		int mWidth;
		int mHeight;
		int mXpos;
		int mYpos;
		bool mProgramRunning;
		bool mHideCursor;
		bool mFullscreen;
		float mDeltaTime;
		long mTickCount;

		void updatePosSize();

		static FreeGlutSystem * s_freeglut_system;
	};

	void onGlutReshape(int width, int height);
	void onGlutDisplay();
	void onGlutKeyboard(unsigned char key, int x, int y);
	void onGlutKeyboardUp(unsigned char key, int x, int y);
	void onGlutSpecialKeyboard(int key, int x, int y);
	void onGlutSpecialKeyboardUp(int key, int x, int y);
	void onGlutMouse(int button, int state, int x, int y);
	void onGlutMouseMove(int x, int y);
	void onGlutIdle();
	
}
}
}
