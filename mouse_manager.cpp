#include "mouse_manager.h"

#include "gfx/gfx_engine.h"
#include "gfx/gfx_renderable.h"
#include "gfx/gfx_layer.h"
#include "gfx/gfx_sprite.h"

namespace am {
namespace ui {

	MouseManager::MouseManager(GfxEngine *engine) :
		mGfxEngine(engine),
		mUnderMouse(NULL)
	{

	}
	MouseManager::~MouseManager()
	{

	}

	void MouseManager::setGfxEngine(GfxEngine *engine)
	{
		mGfxEngine = engine;
	}
	GfxEngine *MouseManager::getGfxEngine() const
	{
		return mGfxEngine;
	}

	void MouseManager::onMouseDown(MouseButton mouseButton, int x, int y)
	{
		mMouseButtonsDown[mouseButton] = true;
		if (!checkForMouseEvent(mGfxEngine->getRootLayer(), "mouse_down", MOUSE_DOWN, mouseButton, x, y, x, y))
		{
			mUnderMouse = NULL;
		}
	}
	void MouseManager::onMouseMove(MouseButton mouseButton, int x, int y)
	{
		if (!checkForMouseEvent(mGfxEngine->getRootLayer(), "mouse_move", MOUSE_MOVE, mouseButton, x, y, x, y))
		{
			if (mUnderMouse != NULL)
			{
				fireMouseEvent(mUnderMouse, "mouse_out", MOUSE_OUT, mouseButton, x, y, -1, -1);
			}
			mUnderMouse = NULL;
		}
	}
	void MouseManager::onMouseUp(MouseButton mouseButton, int x, int y)
	{
		mMouseButtonsDown[mouseButton] = false;
		if (!checkForMouseEvent(mGfxEngine->getRootLayer(), "mouse_up", MOUSE_UP, mouseButton, x, y, x, y))
		{
			mUnderMouse = NULL;
		}
	}

	bool MouseManager::checkForMouseEvent(Renderable *target, const char *type, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY)
	{
		if (target == NULL || !target->getEnableInteractive())
		{
			return false;
		}	

		// TODO: Replace with a multiple by inverse.
		const Matrix4f &localTrans = target->getTransform().getWorldToObj();
		localX -= static_cast<int>(localTrans.wx);
		localY -= static_cast<int>(localTrans.wy);

		Layer *layer = dynamic_cast<Layer *>(target);
		if (layer != NULL)
		{
			int numChildren = layer->getNumChildren();
			for (int i = 0; i < numChildren; i++)
			{
				bool result = checkForMouseEvent(layer->getChildAt(i), type, mouseType, mouseButton, x, y, localX, localY);
				if (result)
				{
					return true;
				}
			}
		}
		else
		{
			if (target->hasEventListener(type) && 
				localX >= 0 && localY >= 0 &&
				localX <= target->getWidth() && localY <= target->getHeight())
			{
				Renderable *oldUnderMouse = mUnderMouse;
				mUnderMouse = target;

				if (target != oldUnderMouse)
				{
					if (oldUnderMouse != NULL)
					{
						fireMouseEvent(mUnderMouse, "mouse_out", MOUSE_OUT, mouseButton, x, y, -1, -1);
					}
					fireMouseEvent(target, "mouse_over", MOUSE_OVER, mouseButton, x, y, localX, localY);
				}
				else
				{
					fireMouseEvent(target, type, mouseType, mouseButton, x, y, localX, localY);
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

	void MouseManager::fireMouseEvent(Renderable *target, const char *type, MouseEventType mouseType, MouseButton mouseButton, int x, int y, int localX, int localY)
	{
		MouseEvent *e = new MouseEvent(type, mouseType, mouseButton, x, y, target, localX, localY);
		while(target != NULL && e->isPropagating())
		{
			target->fireEvent<MouseEvent>(e);
			target = target->getParent();
		}
		delete e;
	}

}
}
