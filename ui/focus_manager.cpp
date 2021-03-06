#include "focus_manager.h"

namespace am {
namespace ui {

	FocusManager *FocusManager::sManager = nullptr;

	FocusManager::FocusManager()
	{

	}
	FocusManager::~FocusManager()
	{

	}

	void FocusManager::setFocus(gfx::Renderable *renderable)
	{
		mCurrentFocus = renderable;
	}
	gfx::Renderable *FocusManager::getFocus() const
	{
		return mCurrentFocus;
	}

	void FocusManager::setManager(FocusManager *manager)
	{
		sManager = manager;
	}
	FocusManager *FocusManager::getManager()
	{
		return sManager;
	}

}
}
