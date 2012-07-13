#include "mouse_manager.h"

#include <gfx/gfx_engine.h>
#include <gfx/gfx_renderable.h>
#include <gfx/gfx_layer.h>
#include <gfx/gfx_sprite.h>

#include <log/logger.h>

#include <game/map.h>

namespace am {
namespace ui {

	MouseManager *MouseManager::sMainManager = NULL;
	
	MouseManager::MouseManager() :
		mUnderMouse(NULL),
		mDragOffsetX(0),
		mDragOffsetY(0)
	{

	}
	MouseManager::~MouseManager()
	{

	}

	void MouseManager::onMouseDown(MouseButton mouseButton, int x, int y)
	{
		mFiredEvent = false;
		mMouseButtonsDown[mouseButton] = true;
		Renderable *hitTarget = checkForMouseEvent(GfxEngine::getEngine()->getRootLayer(), MOUSE_DOWN, mouseButton, x, y, x, y); 
		if (hitTarget == NULL)
		{
			mUnderMouse = NULL;
		}
		if (!mFiredEvent)
		{
			Handle<MouseEvent> e(new MouseEvent(MOUSE_DOWN, mouseButton, x, y));
			fireEvent<MouseEvent>(e.get());
		}
	}
	void MouseManager::onMouseMove(MouseButton mouseButton, int x, int y)
	{
		mFiredEvent = false;
		Renderable *hitTarget = checkForMouseEvent(GfxEngine::getEngine()->getRootLayer(), MOUSE_MOVE, mouseButton, x, y, x, y); 
		if (hitTarget == NULL)
		{
			if (mUnderMouse != NULL)
			{
				fireMouseEvent(mUnderMouse, MOUSE_OUT, mouseButton, x, y, -1, -1);
			}
			mUnderMouse = NULL;
		}
		if (!mFiredEvent)
		{
			Handle<MouseEvent> e(new MouseEvent(MOUSE_MOVE, mouseButton, x, y));
			fireEvent<MouseEvent>(e.get());
		}
	}
	void MouseManager::onMouseUp(MouseButton mouseButton, int x, int y)
	{
		mFiredEvent = false;
		mMouseButtonsDown[mouseButton] = false;
		Renderable *hitTarget = checkForMouseEvent(GfxEngine::getEngine()->getRootLayer(), MOUSE_UP, mouseButton, x, y, x, y);
		if (hitTarget == NULL)
		{
			mUnderMouse = NULL;
		}
		if (!mFiredEvent)
		{
			Handle<MouseEvent> e(new MouseEvent(MOUSE_UP, mouseButton, x, y));
			fireEvent<MouseEvent>(e.get());
		}
	}

	bool MouseManager::getButtonDown(MouseButton button)
	{
		MouseButtonMap::iterator iter = mMouseButtonsDown.find(button);
		if (iter == mMouseButtonsDown.end())
		{
			return false;
		}
		return iter->second;
	}

	Renderable *MouseManager::checkForMouseEvent(Renderable *target, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY)
	{
		if (target == NULL || !target->isInteractive())
		{
			return NULL;
		}	

		// TODO: Replace with a multiple by inverse.
		//const Matrix4f &localTrans = target->getTransform().getWorldToObj();

		localX -= static_cast<int>(target->getPositionX());
		localY -= static_cast<int>(target->getPositionY());

		Layer *layer = dynamic_cast<Layer *>(target);
		if (layer != NULL && layer->isVisible())
		{
			int numChildren = layer->getNumChildren();
			// Loop backwards over children, as that is the order in which
			// they'll appear on screen.
			for (int i = numChildren - 1; i >= 0; i--)
			{
				Renderable *child = layer->getChildAt(i);
				Renderable *hitChild = checkForMouseEvent(child, mouseType, mouseButton, x, y, localX, localY);
				if (hitChild)
				{
					return child;
				}
			}
		}
		else
		{
			if (target->isVisible() &&
				localX >= 0 && localY >= 0 &&
				localX <= target->getWidth() && localY <= target->getHeight())
			{
				Renderable *oldUnderMouse = mUnderMouse;
				mUnderMouse = target;

				bool doFire = target->hasEventListener(MouseEventTypeNames[mouseType]);
				if (target != oldUnderMouse)
				{
					if (oldUnderMouse != NULL)
					{
						fireMouseEvent(oldUnderMouse, MOUSE_OUT, mouseButton, x, y, -1, -1);
					}
					if (doFire)
					{
						fireMouseEvent(target, MOUSE_OVER, mouseButton, x, y, localX, localY);
					}
				}
				else if (doFire)
				{
					fireMouseEvent(target, mouseType, mouseButton, x, y, localX, localY);
				}

				return target;
			}		
		}
		return NULL;
	}

	Renderable *MouseManager::getUnderMouse() const
	{
		return mUnderMouse;
	}
	
	void MouseManager::setDragOffset(int x, int y)
	{
		mDragOffsetX = x;
		mDragOffsetY = y;
	}
	int MouseManager::getDragOffsetX() const
	{
		return mDragOffsetX;
	}
	int MouseManager::getDragOffsetY() const
	{
		return mDragOffsetY;
	}

	void MouseManager::fireMouseEvent(Renderable *target, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY)
	{
		mFiredEvent = true;
		mCurrentEvent = new MouseEvent(mouseType, mouseButton, x, y, target, localX, localY);
		while(target != NULL && mCurrentEvent->isPropagating())
		{
			target->fireEvent<MouseEvent>(mCurrentEvent.get());
			target = target->getParent();
		}
		fireEvent<MouseEvent>(mCurrentEvent.get());
	}

	void MouseManager::setManager(MouseManager *manager)
	{
		sMainManager = manager;
	}
	MouseManager *MouseManager::getManager()
	{
		return sMainManager;
	}

}
}
