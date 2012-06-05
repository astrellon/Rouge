#pragma once

#include "isystem.h"
#include "ievent_listener.h"
#include "mouse_event.h"
#include "mouse_manager.h"

namespace am {
namespace base {
	class Engine;
}
using namespace am::base;

namespace gfx {
	class GfxEngine;
	class TextField;
	class Layer;
}
using namespace am::gfx;

namespace ui {
	class MouseManager;
}
using namespace am::ui;

namespace sys {

	class GameSystem : public ISystem, public am::ui::IEventListener {
	public:
		GameSystem(ISystem *linked, Engine *engine, GfxEngine *gfxEngine,
			MouseManager *mouseManager);
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
		virtual void onKeyDown(const bool *keys, int key);
		virtual void onKeyUp(const bool *keys, int key);

		virtual bool isProgramRunning() const;
		virtual void setProgramRunning(bool running);

		virtual bool isRunning() const;
		virtual int startLoop();
		virtual void stopLoop();

		virtual void onEvent(am::ui::Event *e);
		virtual void onEvent(am::ui::MouseEvent *e);

		virtual void setCursorHidden(bool hide);
		virtual void onCursorHiddenChange(bool hidden);
		virtual bool isCursorHidden() const;

		ISystem *getLinkedSystem();
		GfxEngine *getGfxEngine();
		Engine *getEngine();
		MouseManager *getMouseManager();

	protected:

		ISystem *mLinkedSystem;
		GfxEngine *mGfxEngine;
		Engine *mEngine;
		MouseManager *mMouseManager;

		TextField *mInfo;
		TextField *mInfo2;
		TextField *mInfo3;
	};

}
}
