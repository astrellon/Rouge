#include "mouse_manager.h"

#include <gfx/gfx_renderable.h>
#include <gfx/gfx_layer.h>
#include <gfx/gfx_engine.h>

#include <ui/ui_debug_inspector.h>

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
		Handle<Renderable> hitTarget(checkForMouseEvent(mRootLayer, MOUSE_DOWN, mouseButton, x, y, x, y));
		if (!hitTarget)
		{
			hitTarget = checkForMouseEvent(GfxEngine::getEngine()->getDebugLayer(), MOUSE_DOWN, mouseButton, x, y, x, y);
		}
		DebugInspector::getInspector()->setValue("mouse down", getPath(hitTarget));
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
		Handle<Renderable> hitTarget(checkForMouseEvent(mRootLayer, MOUSE_MOVE, mouseButton, x, y, x, y));
		if (!hitTarget)
		{
			hitTarget = checkForMouseEvent(GfxEngine::getEngine()->getDebugLayer(), MOUSE_MOVE, mouseButton, x, y, x, y);
		}
		DebugInspector::getInspector()->setValue("mouse move", getPath(hitTarget));
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
		Handle<Renderable> hitTarget(checkForMouseEvent(mRootLayer, MOUSE_UP, mouseButton, x, y, x, y));
		if (!hitTarget)
		{
			hitTarget = checkForMouseEvent(GfxEngine::getEngine()->getDebugLayer(), MOUSE_UP, mouseButton, x, y, x, y);
		}
		DebugInspector::getInspector()->setValue("mouse up", getPath(hitTarget));
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

		Handle<Layer> layer(dynamic_cast<Layer *>(target));
		if (layer != nullptr && layer->isVisible() && !layer->interacteWithLayer())
		{
			int numChildren = layer->getNumChildren();
			// Loop backwards over children, as that is the order in which
			// they'll appear on screen.
			for (int i = numChildren - 1; i >= 0; i--)
			{
				Handle<Renderable> child(layer->getChildAt(i));
				Handle<Renderable> hitChild(checkForMouseEvent(child, mouseType, mouseButton, x, y, localX, localY));
				if (hitChild)
				{
					//return child;
					return hitChild;
				}
			}
		}
		else
		{
			if (target->isVisible() &&
				localX >= 0 && localY >= 0 &&
				localX <= target->getWidth() && localY <= target->getHeight())
			{
				Handle<Renderable> oldUnderMouse(mUnderMouse);
				mUnderMouse = target;
				if (target != oldUnderMouse)
				{
					if (oldUnderMouse != nullptr)
					{
						fireMouseEvent(oldUnderMouse, MOUSE_OUT, mouseButton, x, y, -1, -1);
					}
					
					{
						fireMouseEvent(target, MOUSE_OVER, mouseButton, x, y, localX, localY);
					}
				}
				
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
		if (mouseType == MOUSE_DOWN)
		{
			mStopCurrentEvents = false;
		}
		mStopCurrentEvents = false;
		mFiredEvent = true;
		Renderable *inputTarget = target;
		mCurrentEvent = new MouseEvent(mouseType, mouseButton, x, y, inputTarget, localX, localY);
		while (!mStopCurrentEvents && target != nullptr && mCurrentEvent->isPropagating())
		{
			target->fireEvent<MouseEvent>(mCurrentEvent.get());
			target = target->getParent();
			mCurrentEvent->setEventTarget(target);
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

	string MouseManager::getPath(Renderable *target)
	{
		if (!target)
		{
			return "<no target>";
		}
		stringstream ss;
		vector<string> names;
		while (target)
		{
			names.push_back(target->getName());
			target = target->getParent();
		}
		for (size_t i = names.size() - 1; i != -1; i--)
		{
			ss << names[i];
			if (i > 0)
			{
				ss << ", ";
			}
		}
		return ss.str();
	}
}
}
