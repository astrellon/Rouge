#pragma once

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

		virtual void onMouseDown(int mouseButton, int x, int y) = 0;
		virtual void onMouseMove(int mouseButton, int x, int y) = 0;
		virtual void onMouseUp(int mouseButton, int x, int y) = 0;
		virtual void onKeyDown(const bool *keys, int key) = 0;
		virtual void onKeyUp(const bool *keys, int key) = 0;

		virtual bool isProgramRunning() const = 0;
		virtual void setProgramRunning(bool running) = 0;

		virtual bool isRunning() const = 0;
		virtual int startLoop() = 0;
		virtual void stopLoop() = 0;

	};

}
}

