#include "character.h"

#include "engine.h"

#include <sstream>
using namespace std;

namespace am {
namespace game {

	Character::Character() :
		GameObject(),
		mGraphic(NULL),
		mController(NULL),
		mMoveX(0),
		mMoveY(0)
	{
		mInfo = new TextField();
		mInfo->setBaseFont("basic");
		addChild(mInfo.get());
		setName("Character");
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
		mInfo->setPosition(graphic->getWidth() + 2.0f, 0.0f);
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
		if (mMoveX != 0 || mMoveY != 0)
		{
			Engine *engine = Engine::getEngine();
			float dx = engine->getGridXSize() * static_cast<float>(mMoveX);
			float dy = engine->getGridYSize() * static_cast<float>(mMoveY);
			stringstream ss;
			ss << "(" << dx << ", " << dy << ")";
			mInfo->setText(ss.str().c_str());
			move(dx, dy);
		}
		else
		{
			mInfo->setText("(0, 0)");
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

	void Character::move(float x, float y)
	{
		setLocation(mLocationX + x, mLocationY + y);
	}

	void Character::setMoveVector(int x, int y)
	{
		mMoveX = x;
		mMoveY = y;
	}
	int Character::getMoveVectorX() const
	{
		return mMoveX;
	}
	int Character::getMoveVectorY() const
	{
		return mMoveY;
	}

}
}
