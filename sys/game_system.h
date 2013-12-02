#pragma once

#include <base/handle.h>

#include <ui/ievent_listener.h>
#include <ui/mouse_event.h>
#include <ui/mouse_manager.h>
#include <ui/ui_debug_inspector.h>
#include <gfx/gfx_tooltip.h>
#include "isystem.h"

namespace am {
namespace game {
	class Engine;
}

namespace gfx {
	class GfxEngine;
	class Layer;
	class TextList;
	class GfxLogListener;
}

namespace ui {
	class MouseManager;
}

namespace sys {
	
	class OsSystem;

	class GameSystem : public ISystem, public ui::IEventListener 
	{
	public:
		
		~GameSystem();

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
		virtual void onMouseMove(ui::Mouse::Button mouseButton, int x, int y);
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

		virtual OsSystem *getLinkedSystem();
		virtual Engine *getEngine();

		virtual bool isDirectory(const char *folderName);
		virtual bool isFile(const char *filename);
		virtual bool createDirectory(const char *folderName);
		virtual base::ReturnCode listDirectory(const char *folderName, FolderEntryList &result);

		virtual void onEvent(ui::MouseEvent *e);

		virtual gfx::Tooltip *getDefaultTooltip() const;

		static GameSystem *createGameSystem(OsSystem *linked, Engine *engine);
		static GameSystem *getGameSystem();

		TextList *getDebugConsole();

	protected:

		GameSystem(OsSystem *linked, Engine *engine);

		OsSystem *mLinkedSystem;
		game::Engine *mEngine;

		base::Handle<gfx::GfxLogListener> mGfxListener;

		static GameSystem *sGameSystem;

		base::Handle<gfx::TextList> mDebugConsole;
		base::Handle<ui::DebugInspector> mDebugInspector;
		base::Handle<gfx::Tooltip> mTooltip;
	};

}
}
