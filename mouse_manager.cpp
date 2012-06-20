#include "mouse_manager.h"

#include "gfx/gfx_engine.h"
#include "gfx/gfx_renderable.h"
#include "gfx/gfx_layer.h"
#include "gfx/gfx_sprite.h"

namespace am {
namespace ui {

	MouseManager *MouseManager::sMainManager = NULL;

	MouseManager::MouseManager() :
		mUnderMouse(NULL)
	{

	}
	MouseManager::~MouseManager()
	{

	}

	void MouseManager::onMouseDown(MouseButton mouseButton, int x, int y)
	{
		mMouseButtonsDown[mouseButton] = true;
		if (!checkForMouseEvent(GfxEngine::getGfxEngine()->getRootLayer(), MOUSE_DOWN, mouseButton, x, y, x, y))
		{
			mUnderMouse = NULL;
		}
	}
	void MouseManager::onMouseMove(MouseButton mouseButton, int x, int y)
	{
		if (!checkForMouseEvent(GfxEngine::getGfxEngine()->getRootLayer(), MOUSE_MOVE, mouseButton, x, y, x, y))
		{
			if (mUnderMouse != NULL)
			{
				fireMouseEvent(mUnderMouse, MOUSE_OUT, mouseButton, x, y, -1, -1);
			}
			mUnderMouse = NULL;
		}
	}
	void MouseManager::onMouseUp(MouseButton mouseButton, int x, int y)
	{
		mMouseButtonsDown[mouseButton] = false;
		if (!checkForMouseEvent(GfxEngine::getGfxEngine()->getRootLayer(), MOUSE_UP, mouseButton, x, y, x, y))
		{
			mUnderMouse = NULL;
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

	bool MouseManager::checkForMouseEvent(Renderable *target, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY)
	{
		if (target == NULL || !target->isInteractive())
		{
			return false;
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
				bool result = checkForMouseEvent(layer->getChildAt(i), mouseType, mouseButton, x, y, localX, localY);
				if (result)
				{
					return true;
				}
			}
		}
		else
		{
			if (target->isVisible() &&
				target->hasEventListener(MouseEventTypeNames[mouseType]) && 
				localX >= 0 && localY >= 0 &&
				localX <= target->getWidth() && localY <= target->getHeight())
			{
				Renderable *oldUnderMouse = mUnderMouse;
				mUnderMouse = target;

				if (target != oldUnderMouse)
				{
					if (oldUnderMouse != NULL)
					{
						fireMouseEvent(oldUnderMouse, MOUSE_OUT, mouseButton, x, y, -1, -1);
					}
					fireMouseEvent(target, MOUSE_OVER, mouseButton, x, y, localX, localY);
				}
				else
				{
					fireMouseEvent(target, mouseType, mouseButton, x, y, localX, localY);
				}

				return true;
			}		
		}
		return false;
	}

	Renderable *MouseManager::getUnderMouse() const
	{
		return mUnderMouse;
	}

	void MouseManager::fireMouseEvent(Renderable *target, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY)
	{
		Handle<MouseEvent> e(new MouseEvent(mouseType, mouseButton, x, y, target, localX, localY));
		while(target != NULL && e->isPropagating())
		{
			target->fireEvent<MouseEvent>(e.get());
			target = target->getParent();
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

}
}
