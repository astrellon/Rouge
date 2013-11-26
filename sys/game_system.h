#pragma once

#include <base/handle.h>
using namespace am::base;

#include <ui/ievent_listener.h>
#include <ui/mouse_event.h>
#include <ui/mouse_manager.h>
#include <ui/ui_debug_inspector.h>
//#include <ui/ui_tooltip.h>
//#include <ui/ui_item_tooltip.h>
#include <gfx/gfx_tooltip.h>
using namespace am::ui;

#include "isystem.h"

namespace am {
namespace game {
	class Engine;
}
using namespace am::game;

namespace gfx {
	class GfxEngine;
	class Layer;
	class TextList;
	class GfxLogListener;
}
using namespace am::gfx;

namespace ui {
	class MouseManager;
}

namespace sys {
	
	class OsSystem;

	class GameSystem : public ISystem, public am::ui::IEventListener {
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

		virtual void onMouseDown(am::ui::MouseButton mouseButton, int x, int y);
		virtual void onMouseMove(am::ui::MouseButton mouseButton, int x, int y);
		virtual void onMouseUp(am::ui::MouseButton mouseButton, int x, int y);
		virtual void onKeyDown(Key key);
		virtual void onKeyUp(Key key);
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

		virtual void onEvent(MouseEvent *e);

		virtual Tooltip *getDefaultTooltip() const;

		static GameSystem *createGameSystem(OsSystem *linked, Engine *engine);
		static GameSystem *getGameSystem();

		TextList *getDebugConsole();

	protected:

		GameSystem(OsSystem *linked, Engine *engine);

		OsSystem *mLinkedSystem;
		Engine *mEngine;

		Handle<GfxLogListener> mGfxListener;

		static GameSystem *sGameSystem;

		Handle<TextList> mDebugConsole;
		Handle<DebugInspector> mDebugInspector;
		Handle<Tooltip> mTooltip;
	};

}
}
