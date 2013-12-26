#pragma once

#include <map>

#include <base/handle.h>

#include "mouse_common.h"
#include "mouse_event.h"
#include "event_interface.h"

namespace am {
namespace gfx {
	class Renderable;
	class Layer;
}

namespace ui {

	class MouseManager : public EventInterface
	{
	public:

		MouseManager();
		~MouseManager();

		gfx::Renderable *checkForMouseEvent(gfx::Renderable *target, Mouse::EventType mouseType, Mouse::Button mouseButton, int x, int y, int localX, int localY);
		gfx::Renderable *getUnderMouse() const;
		bool getButtonDown(Mouse::Button button);

		void onMouseDown(Mouse::Button mouseButton, int x, int y);
		void onMouseMove(int x, int y);
		void onMouseUp(Mouse::Button mouseButton, int x, int y);

		void setDragOffset(int x, int y);
		int getDragOffsetX() const;
		int getDragOffsetY() const;

		void clearCurrentlyFiring();

		void fireMouseEvent(gfx::Renderable *target, Mouse::EventType mouseType, Mouse::Button mouseButton, int x, int y, int localX, int localY);

		void setRootLayer(gfx::Layer *layer);
		gfx::Layer *getRootLayer() const;

		int getMouseX() const;
		int getMouseY() const;

		static void setManager(MouseManager *manager);
		static MouseManager *getManager();

	protected:

		base::Handle<gfx::Renderable> mUnderMouse;
		base::Handle<gfx::Layer> mRootLayer;

		bool mStopCurrentEvents;
		bool mFiredEvent;
		base::Handle<MouseEvent> mCurrentEvent;
		typedef std::map<Mouse::Button, bool> MouseButtonMap;
		MouseButtonMap mMouseButtonsDown;

		int mDragOffsetX;
		int mDragOffsetY;

		int mMouseX;
		int mMouseY;

		static MouseManager *sMainManager;

		static std::string getPath(gfx::Renderable *target);
	};

}
}
