#include "character.h"

namespace am {
namespace base {

	Character::Character() :
		GameObject(),
		mGraphic(NULL),
		mController(NULL)
	{

	}
	Character::~Character()
	{

	}

	void Character::setGraphic(Sprite *graphic)
	{
		if (mGraphic.get())
		{
			removeChild(mGraphic.get());
		}
		mGraphic = graphic;
		if (graphic)
		{
			addChild(graphic);
			// Aim for head-ish area.
			mCameraOffsetX = graphic->getWidth() * 0.5f;
			mCameraOffsetY = graphic->getHeight() * 0.3f;
		}
	}
	Sprite *Character::getGraphic()
	{
		return mGraphic.get();
	}

	void Character::update(float dt)
	{
		if (mController.get())
		{
			mController->update(this, dt);
		}
	}

	void Character::setName(const char *name)
	{
		mName = name;
	}
	void Character::setName(const string &name)
	{
		mName = name;
	}
	string Character::getName() const
	{
		return mName;
	}

	void Character::setController(IController *controller)
	{
		mController = controller;
	}
	IController *Character::getController()
	{
		return mController.get();
	}

}
}
