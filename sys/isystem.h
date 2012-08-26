#pragma once

#include "../ui/mouse_event.h"
#include "../ui/mouse_manager.h"

namespace am {
namespace sys {

	class ISystem {
	public:
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

		virtual void onMouseDown(am::ui::MouseButton mouseButton, int x, int y) = 0;
		virtual void onMouseMove(am::ui::MouseButton mouseButton, int x, int y) = 0;
		virtual void onMouseUp(am::ui::MouseButton mouseButton, int x, int y) = 0;
		virtual void onKeyDown(int key, bool systemKey) = 0;
		virtual void onKeyUp(int key) = 0;

		virtual bool isProgramRunning() const = 0;
		virtual void setProgramRunning(bool running) = 0;

		virtual bool isRunning() const = 0;
		virtual int startLoop() = 0;
		virtual void stopLoop() = 0;

		virtual void setCursorHidden(bool hide) = 0;
		virtual void onCursorHiddenChange(bool hidden) = 0;
		virtual bool isCursorHidden() const = 0;

		virtual void setFullscreen(bool fullscreen) = 0;
		virtual bool getFullscreen() const = 0;

	};

}
}

