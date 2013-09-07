#pragma once

#include <map>
using namespace std;

#include <base/handle.h>
using namespace am::base;

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

		Renderable *checkForMouseEvent(Renderable *target, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY);
		Renderable *getUnderMouse() const;
		bool getButtonDown(MouseButton button);

		void onMouseDown(MouseButton mouseButton, int x, int y);
		void onMouseMove(MouseButton mouseButton, int x, int y);
		void onMouseUp(MouseButton mouseButton, int x, int y);

		void setDragOffset(int x, int y);
		int getDragOffsetX() const;
		int getDragOffsetY() const;

		void clearCurrentlyFiring();

		void fireMouseEvent(Renderable *target, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY);

		void setRootLayer(Layer *layer);
		Layer *getRootLayer() const;

		int getMouseX() const;
		int getMouseY() const;

		static void setManager(MouseManager *manager);
		static MouseManager *getManager();

	protected:

		Handle<Renderable> mUnderMouse;
		Handle<Layer> mRootLayer;

		bool mStopCurrentEvents;
		bool mFiredEvent;
		Handle<MouseEvent> mCurrentEvent;
		typedef map<MouseButton, bool> MouseButtonMap;
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
