#include "mouse_manager.h"

#include <gfx/gfx_renderable.h>
#include <gfx/gfx_layer.h>
#include <gfx/gfx_engine.h>
#include <gfx/gfx_tooltip.h>
#include <gfx/gfx_camera.h>

#include <ui/ui_debug_inspector.h>

#include <log/logger.h>

namespace am {
namespace ui {

	MouseManager *MouseManager::sMainManager = nullptr;
	
	MouseManager::MouseManager() :
		mDragOffsetX(0),
		mDragOffsetY(0),
		mMouseX(0),
		mMouseY(0),
		mGfxEngine(nullptr)
	{

	}
	MouseManager::~MouseManager()
	{

	}

	void MouseManager::onMouseDown(ui::Mouse::Button mouseButton, int x, int y)
	{
		mMouseX = x;
		mMouseY = y;
		mFiredEvent = false;
		mMouseButtonsDown[mouseButton] = true;
		base::Handle<gfx::Renderable> hitTarget(checkForMouseEvent(mGfxEngine->getUILayer(), ui::Mouse::MOUSE_DOWN, mouseButton, x, y, x, y));
		if (!hitTarget)
		{
			hitTarget = checkForMouseEventAdj(mGfxEngine->getGameLayer(), ui::Mouse::MOUSE_DOWN, mouseButton, x, y);
		}
		DebugInspector::getInspector()->setValue("mouse down", getPath(hitTarget));
		if (hitTarget == nullptr)
		{
			mUnderMouse = nullptr;
		}
		if (!mFiredEvent)
		{
			base::Handle<MouseEvent> e(new MouseEvent(ui::Mouse::MOUSE_DOWN, mouseButton, x, y));
			fireEvent<MouseEvent>(e.get());
		}
	}
	void MouseManager::onMouseMove(int x, int y)
	{
        ui::Mouse::Button mouseButton = ui::Mouse::NO_BUTTON;
		mMouseX = x;
		mMouseY = y;
		mFiredEvent = false;

		base::Handle<gfx::Renderable> hitTarget(checkForMouseEvent(mGfxEngine->getUILayer(), ui::Mouse::MOUSE_MOVE, mouseButton, x, y, x, y));
		if (!hitTarget)
		{
			hitTarget = checkForMouseEventAdj(mGfxEngine->getGameLayer(), ui::Mouse::MOUSE_MOVE, mouseButton, x, y);
		}
		DebugInspector::getInspector()->setValue("mouse move", getPath(hitTarget));
		if (hitTarget == nullptr)
		{
			if (mUnderMouse != nullptr)
			{
				fireMouseEvent(mUnderMouse, ui::Mouse::MOUSE_OUT, mouseButton, x, y, -1, -1);
			}
			mUnderMouse = nullptr;
		}
		if (!mFiredEvent)
		{
			base::Handle<MouseEvent> e(new MouseEvent(ui::Mouse::MOUSE_MOVE, mouseButton, x, y));
			fireEvent<MouseEvent>(e.get());
		}
	}
	void MouseManager::onMouseUp(ui::Mouse::Button mouseButton, int x, int y)
	{
		mMouseX = x;
		mMouseY = y;
		mFiredEvent = false;
		mMouseButtonsDown[mouseButton] = false;
		base::Handle<gfx::Renderable> hitTarget(checkForMouseEvent(mGfxEngine->getUILayer(), ui::Mouse::MOUSE_UP, mouseButton, x, y, x, y));
		if (!hitTarget)
		{
			hitTarget = checkForMouseEventAdj(mGfxEngine->getGameLayer(), ui::Mouse::MOUSE_UP, mouseButton, x, y);
		}
		DebugInspector::getInspector()->setValue("mouse up", getPath(hitTarget));
		if (hitTarget == nullptr)
		{
			mUnderMouse = nullptr;
		}
		if (!mFiredEvent)
		{
			base::Handle<MouseEvent> e(new MouseEvent(ui::Mouse::MOUSE_UP, mouseButton, x, y));
			fireEvent<MouseEvent>(e.get());
		}
	}

	bool MouseManager::getButtonDown(ui::Mouse::Button button)
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

	gfx::Renderable *MouseManager::checkForMouseEvent(gfx::Renderable *target, ui::Mouse::EventType mouseType, ui::Mouse::Button mouseButton, int x, int y, int localX, int localY)
	{
		if (target == nullptr || !target->isInteractive())
		{
			return nullptr;
		}

		// TODO: Replace with a multiple by inverse.
		//const Matrix4f &localTrans = target->getTransform().getWorldToObj();

		localX -= static_cast<int>(target->getPositionX());
		localY -= static_cast<int>(target->getPositionY());

		base::Handle<gfx::Layer> layer(dynamic_cast<gfx::Layer *>(target));
		if (layer != nullptr && layer->isVisible() && !layer->interactWithLayer())
		{
			int numChildren = layer->getNumChildren();
			// Loop backwards over children, as that is the order in which
			// they'll appear on screen.
			for (int i = numChildren - 1; i >= 0; i--)
			{
				base::Handle<gfx::Renderable> child(layer->getChildAt(i));
				base::Handle<gfx::Renderable> hitChild(checkForMouseEvent(child, mouseType, mouseButton, x, y, localX, localY));
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
				base::Handle<gfx::Renderable> oldUnderMouse(mUnderMouse);
				mUnderMouse = target;
				if (target != oldUnderMouse)
				{
					if (oldUnderMouse != nullptr)
					{
						fireMouseEvent(oldUnderMouse, ui::Mouse::MOUSE_OUT, mouseButton, x, y, -1, -1);
					}
					
					fireMouseEvent(target, ui::Mouse::MOUSE_OVER, mouseButton, x, y, localX, localY);
				}
				
				{
					fireMouseEvent(target, mouseType, mouseButton, x, y, localX, localY);
				}

				return target;
			}		
		}
		return nullptr;
	}

	gfx::Renderable *MouseManager::checkForMouseEventAdj(gfx::Renderable *target, ui::Mouse::EventType mouseType, ui::Mouse::Button mouseButton, int x, int y)
	{
		int localX = x + mGfxEngine->getScreenWidth() / 2;
		int localY = y + mGfxEngine->getScreenHeight() / 2;
		return checkForMouseEvent(target, mouseType, mouseButton, x, y, localX, localY);
	}
	gfx::Renderable *MouseManager::getUnderMouse() const
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

	void MouseManager::fireMouseEvent(gfx::Renderable *target, ui::Mouse::EventType mouseType, ui::Mouse::Button mouseButton, int x, int y, int localX, int localY)
	{
		if (mouseType == ui::Mouse::MOUSE_DOWN)
		{
			mStopCurrentEvents = false;
		}
		mStopCurrentEvents = false;
		mFiredEvent = true;
		gfx::Renderable *inputTarget = target;
		mCurrentEvent = new MouseEvent(mouseType, mouseButton, x, y, inputTarget, localX, localY);
		while (!mStopCurrentEvents && target != nullptr && mCurrentEvent->isPropagating())
		{
			target->fireEvent<MouseEvent>(mCurrentEvent.get());
			if (mouseType == ui::Mouse::MOUSE_OVER && target->hasTooltip())
			{
				target->getTooltip()->onMouseOver(x, y);
			}
			else if (mouseType == ui::Mouse::MOUSE_OUT && target->hasTooltip())
			{
				target->getTooltip()->onMouseOut(x, y);
			}
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

	void MouseManager::setGfxEngine(gfx::GfxEngine *engine)
	{
		mGfxEngine = engine;
	}
	gfx::GfxEngine *MouseManager::getGfxEngine() const
	{
		return mGfxEngine;
	}

	std::string MouseManager::getPath(gfx::Renderable *target)
	{
		if (!target)
		{
			return "<no target>";
		}
		std::stringstream ss;
		std::vector<std::string> names;
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
