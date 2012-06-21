#include "character.h"

#include <sstream>
using namespace std;

namespace am {
namespace base {

	Character::Character() :
		GameObject(),
		mGraphic(NULL),
		mController(NULL),
		// Approx 4km/h
		mWalkingSpeed(1.2f),
		// Approx 10km/h
		mRunningSpeed(3.0f),
		// Approx 20km/h/s
		mAcceleration(6.0f),
		mRunning(false),
		mMoveX(0.0f),
		mMoveY(0.0f)
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
		if (mMoveX != 0.0f || mMoveY != 0.0f)
		{
			float topSpeed = mRunning ? mRunningSpeed : mWalkingSpeed;
			mCurrentSpeed += mAcceleration * dt;
			mCurrentSpeed = mCurrentSpeed > topSpeed ? topSpeed : mCurrentSpeed;

			float dx = mMoveX * mCurrentSpeed;
			float dy = mMoveY * mCurrentSpeed;
			stringstream ss;
			ss << "(" << dx << ", " << dy << ")";
			mInfo->setText(ss.str().c_str());
			move(dx, dy);
		}
		else
		{
			mInfo->setText("(0, 0)");
			mCurrentSpeed = 0.0f;
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
		setGameLocation(mGameLocationX + x, mGameLocationY + y);
	}

	void Character::setMoveVector(float x, float y)
	{
		mMoveX = x;
		mMoveY = y;
	}
	float Character::getMoveVectorX() const
	{
		return mMoveX;
	}
	float Character::getMoveVectorY() const
	{
		return mMoveY;
	}

	void Character::setWalkingSpeed(float speed)
	{
		mWalkingSpeed = speed;
	}
	float Character::getWalkingSpeed() const
	{
		return mWalkingSpeed;
	}

	void Character::setRunningSpeed(float speed)
	{
		mRunningSpeed = speed;
	}
	float Character::getRunningSpeed() const
	{
		return mRunningSpeed;
	}

	void Character::setAcceleration(float acc)
	{
		mAcceleration = acc;
	}
	float Character::getAcceleration() const
	{
		return mAcceleration;
	}

	void Character::setRunning(bool running)
	{
		mRunning = running;
	}
	bool Character::isRunning() const
	{
		return mRunning;
	}

	float Character::getCurrentSpeed()
	{
		return mCurrentSpeed;
	}

}
}
