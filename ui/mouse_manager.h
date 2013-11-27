#pragma once

#include <map>
using namespace std;

#include <base/handle.h>

#include "mouse_common.h"
#include "mouse_event.h"
#include "event_interface.h"

namespace am {
namespace gfx {
	class Renderable;
	class Layer;
}

using namespace am::gfx;

namespace ui {

	class MouseManager : public EventInterface {
	public:

		MouseManager();
		~MouseManager();

		Renderable *checkForMouseEvent(Renderable *target, Mouse::EventType mouseType, Mouse::Button mouseButton, int x, int y, int localX, int localY);
		Renderable *getUnderMouse() const;
		bool getButtonDown(Mouse::Button button);

		void onMouseDown(Mouse::Button mouseButton, int x, int y);
		void onMouseMove(Mouse::Button mouseButton, int x, int y);
		void onMouseUp(Mouse::Button mouseButton, int x, int y);

		void setDragOffset(int x, int y);
		int getDragOffsetX() const;
		int getDragOffsetY() const;

		void clearCurrentlyFiring();

		void fireMouseEvent(Renderable *target, Mouse::EventType mouseType, Mouse::Button mouseButton, int x, int y, int localX, int localY);

		void setRootLayer(Layer *layer);
		Layer *getRootLayer() const;

		int getMouseX() const;
		int getMouseY() const;

		static void setManager(MouseManager *manager);
		static MouseManager *getManager();

	protected:

		am::base::Handle<Renderable> mUnderMouse;
		am::base::Handle<Layer> mRootLayer;

		bool mStopCurrentEvents;
		bool mFiredEvent;
		am::base::Handle<MouseEvent> mCurrentEvent;
		typedef map<Mouse::Button, bool> MouseButtonMap;
		MouseButtonMap mMouseButtonsDown;

		int mDragOffsetX;
		int mDragOffsetY;

		int mMouseX;
		int mMouseY;

		static MouseManager *sMainManager;

		static string getPath(Renderable *target);
	};

}
}
