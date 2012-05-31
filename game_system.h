#pragma once

#include "isystem.h"
#include "ievent_listener.h"
#include "mouse_event.h"

namespace am {
namespace base {
	class Engine;
}

using namespace am::base;

namespace gfx{
	class GfxEngine;
	class TextField;
}

using namespace am::gfx;

namespace sys {

	class GameSystem : public ISystem, public am::ui::IEventListener {
	public:
		GameSystem(ISystem *linked, Engine *engine, GfxEngine *gfxEngine);
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

		virtual void onMouseDown(int mouseButton, int x, int y);
		virtual void onMouseMove(int mouseButton, int x, int y);
		virtual void onMouseUp(int mouseButton, int x, int y);
		virtual void onKeyDown(const bool *keys, int key);
		virtual void onKeyUp(const bool *keys, int key);

		virtual bool isProgramRunning() const;
		virtual void setProgramRunning(bool running);

		virtual bool isRunning() const;
		virtual int startLoop();
		virtual void stopLoop();

		virtual void onEvent(am::ui::MouseEvent &e);

		ISystem *getLinkedSystem();
		GfxEngine *getGfxEngine();
		Engine *getEngine();

	protected:

		ISystem *mLinkedSystem;
		GfxEngine *mGfxEngine;
		Engine *mEngine;

		TextField *mInfo;
	};

}
}
