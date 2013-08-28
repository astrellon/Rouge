#include "mouse_manager.h"

#include <gfx/gfx_renderable.h>
#include <gfx/gfx_layer.h>

#include <log/logger.h>

namespace am {
namespace ui {

	MouseManager *MouseManager::sMainManager = nullptr;
	
	MouseManager::MouseManager() :
		mDragOffsetX(0),
		mDragOffsetY(0),
		mMouseX(0),
		mMouseY(0)
	{

	}
	MouseManager::~MouseManager()
	{

	}

	void MouseManager::onMouseDown(MouseButton mouseButton, int x, int y)
	{
		mMouseX = x;
		mMouseY = y;
		mFiredEvent = false;
		mMouseButtonsDown[mouseButton] = true;
		Renderable *hitTarget = checkForMouseEvent(mRootLayer, MOUSE_DOWN, mouseButton, x, y, x, y); 
		if (hitTarget == nullptr)
		{
			mUnderMouse = nullptr;
		}
		if (!mFiredEvent)
		{
			Handle<MouseEvent> e(new MouseEvent(MOUSE_DOWN, mouseButton, x, y));
			fireEvent<MouseEvent>(e.get());
		}
	}
	void MouseManager::onMouseMove(MouseButton mouseButton, int x, int y)
	{
		mMouseX = x;
		mMouseY = y;
		mFiredEvent = false;
		Renderable *hitTarget = checkForMouseEvent(mRootLayer, MOUSE_MOVE, mouseButton, x, y, x, y); 
		if (hitTarget == nullptr)
		{
			if (mUnderMouse != nullptr)
			{
				fireMouseEvent(mUnderMouse, MOUSE_OUT, mouseButton, x, y, -1, -1);
			}
			mUnderMouse = nullptr;
		}
		if (!mFiredEvent)
		{
			Handle<MouseEvent> e(new MouseEvent(MOUSE_MOVE, mouseButton, x, y));
			fireEvent<MouseEvent>(e.get());
		}
	}
	void MouseManager::onMouseUp(MouseButton mouseButton, int x, int y)
	{
		mMouseX = x;
		mMouseY = y;
		mFiredEvent = false;
		mMouseButtonsDown[mouseButton] = false;
		Renderable *hitTarget = checkForMouseEvent(mRootLayer, MOUSE_UP, mouseButton, x, y, x, y);
		if (hitTarget == nullptr)
		{
			mUnderMouse = nullptr;
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

	int MouseManager::getMouseX() const
	{
		return mMouseX;
	}
	int MouseManager::getMouseY() const
	{
		return mMouseY;
	}

	Renderable *MouseManager::checkForMouseEvent(Renderable *target, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY)
	{
		if (target == nullptr || !target->isInteractive())
		{
			return nullptr;
		}

		// TODO: Replace with a multiple by inverse.
		//const Matrix4f &localTrans = target->getTransform().getWorldToObj();

		localX -= static_cast<int>(target->getPositionX());
		localY -= static_cast<int>(target->getPositionY());

		Layer *layer = dynamic_cast<Layer *>(target);
		if (layer != nullptr && layer->isVisible() && !layer->interacteWithLayer())
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
				//bool doFire = target->hasEventListener(MouseEventTypeNames[mouseType]);
				if (target != oldUnderMouse)
				{
					if (oldUnderMouse != nullptr)
					{
						fireMouseEvent(oldUnderMouse, MOUSE_OUT, mouseButton, x, y, -1, -1);
					}
					//if (doFire)
					{
						fireMouseEvent(target, MOUSE_OVER, mouseButton, x, y, localX, localY);
					}
				}
				//else// if (doFire)
				{
					fireMouseEvent(target, mouseType, mouseButton, x, y, localX, localY);
				}

				return target;
			}		
		}
		return nullptr;
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
	
	void MouseManager::clearCurrentlyFiring()
	{
		mStopCurrentEvents = true;
	}

	void MouseManager::fireMouseEvent(Renderable *target, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY)
	{
		mStopCurrentEvents = false;
		mFiredEvent = true;
		mCurrentEvent = new MouseEvent(mouseType, mouseButton, x, y, target, localX, localY);
		while(!mStopCurrentEvents && target != nullptr && mCurrentEvent->isPropagating())
		{
			target->fireEvent<MouseEvent>(mCurrentEvent.get());
			target = target->getParent();
		}
		if (!mStopCurrentEvents)
		{
			fireEvent<MouseEvent>(mCurrentEvent.get());
		}
	}

	void MouseManager::setManager(MouseManager *manager)
	{
		sMainManager = manager;
	}
	MouseManager *MouseManager::getManager()
	{
		return sMainManager;
	}

	void MouseManager::setRootLayer( Layer *layer )
	{
		mRootLayer = layer;
	}
	Layer *MouseManager::getRootLayer() const
	{
		return mRootLayer;
	}

}
}
