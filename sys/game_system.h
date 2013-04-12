#pragma once

#include "isystem.h"
#include "../ui/ievent_listener.h"
#include "../ui/mouse_event.h"
#include "../ui/mouse_manager.h"
#include "../base/handle.h"
using namespace am::base;

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
using namespace am::ui;

namespace sys {

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
		virtual void onKeyDown(int key, bool systemKey);
		virtual void onKeyUp(int key);

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

		virtual ISystem *getLinkedSystem();
		virtual Engine *getEngine();

		virtual bool isDirectory(const char *folderName);
		virtual bool createDirectory(const char *folderName);

		static GameSystem *createGameSystem(ISystem *linked, Engine *engine);
		static GameSystem *getGameSystem();

		TextList *getDebugConsole();

	protected:

		GameSystem(ISystem *linked, Engine *engine);

		ISystem *mLinkedSystem;
		Engine *mEngine;

		GfxLogListener *mGfxListener;

		static GameSystem *sGameSystem;

		Handle<TextList> mDebugConsole;
	};

}
}
