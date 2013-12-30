#pragma once

#include <base/return_codes.h>

#include <ui/mouse_event.h>
#include <ui/mouse_manager.h>
#include <ui/keyboard_common.h>

namespace am {
namespace sys {

	class ISystem
	{
	public:

		typedef struct 
		{
			std::string name;
			bool isDirectory;
		} FolderEntry;

		typedef std::vector<FolderEntry> FolderEntryList;
		virtual void setSize(int width, int height) = 0;
		virtual void setPosition(int x, int y) = 0;

		virtual int getWidth() const = 0;
		virtual int getHeight() const = 0;

		virtual int getX() const = 0;
		virtual int getY() const = 0;

		virtual void setTitle(const char *title) = 0;
		virtual const char *getTitle() const = 0;

		virtual void init() = 0;
		virtual void reshape(int width, int height) = 0;
		virtual void update(float dt) = 0;
		virtual void display(float dt) = 0;
		virtual void deinit() = 0;

		virtual void onMouseDown(ui::Mouse::Button mouseButton, int x, int y) = 0;
		virtual void onMouseMove(int x, int y) = 0;
		virtual void onMouseUp(ui::Mouse::Button mouseButton, int x, int y) = 0;
		virtual void onKeyDown(ui::Keyboard::Key key) = 0;
		virtual void onKeyUp(ui::Keyboard::Key key) = 0;
		virtual void onKeyPress(char key) = 0;

		virtual bool isProgramRunning() const = 0;
		virtual void setProgramRunning(bool running) = 0;

		virtual int startLoop(int argc, char **argv) = 0;
		virtual void stopLoop() = 0;

		virtual void setCursorHidden(bool hide) = 0;
		virtual void onCursorHiddenChange(bool hidden) = 0;
		virtual bool isCursorHidden() const = 0;

		virtual void setFullscreen(bool fullscreen) = 0;
		virtual bool getFullscreen() const = 0;

		virtual bool isDirectory(const char *folderName) = 0;
		virtual bool isFile(const char *filename) = 0;
		virtual bool createDirectory(const char *folderName) = 0;
		virtual base::ReturnCode listDirectory(const char *folderName, FolderEntryList &result) = 0;

	};

}
}

