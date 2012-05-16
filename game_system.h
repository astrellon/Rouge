#pragma once

#include "isystem.h"

namespace am {
namespace base {
	class Engine;
}

using namespace am::base;

namespace gfx{
	class IGfxEngine;
}

using namespace am::gfx;

namespace sys {

	class GameSystem : public ISystem {
	public:
		GameSystem(ISystem *linked, Engine *engine, IGfxEngine *gfxEngine);
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
		virtual void update(unsigned long dt);
		virtual void display();
		virtual void deinit();

		virtual bool isProgramRunning() const;
		virtual void setProgramRunning(bool running);

		virtual bool isRunning() const;
		virtual int startLoop();
		virtual void stopLoop();

		ISystem *getLinkedSystem();
		IGfxEngine *getGfxEngine();
		Engine *getEngine();

	protected:

		ISystem *mLinkedSystem;
		IGfxEngine *mGfxEngine;
		Engine *mEngine;
	};

}
}
