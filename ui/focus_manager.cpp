#include "focus_manager.h"

namespace am {
namespace ui {

	FocusManager *FocusManager::sManager = NULL;

	FocusManager::FocusManager()
	{

	}
	FocusManager::~FocusManager()
	{

	}

	void FocusManager::setFocus(Renderable *renderable)
	{
		mCurrentFocus = renderable;
	}
	Renderable *FocusManager::getFocus() const
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
