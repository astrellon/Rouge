#pragma once

#include "game_object.h"
#include "handle.h"
using namespace am::util;

#include <string>
using namespace std;

#include "gfx/gfx_text_field.h"
#include "gfx/gfx_sprite.h"
using namespace am::gfx;

#include "icontroller.h"

namespace am {
namespace base {

	class Character : public GameObject {
	public:
		Character();
		~Character();

		void setGraphic(Sprite *graphic);
		Sprite *getGraphic();

		void setName(const char *name);
		void setName(const string &name);
		string getName() const;

		virtual void update(float dt);

		virtual void move(float x, float y);

		virtual void setWalkingSpeed(float speed);
		virtual float getWalkingSpeed() const;

		virtual void setRunningSpeed(float speed);
		virtual float getRunningSpeed() const;

		virtual void setAcceleration(float acc);
		virtual float getAcceleration() const;

		virtual void setRunning(bool running);
		virtual bool isRunning() const;

		virtual void setController(IController *controller);
		virtual IController *getController();

		virtual void setMoveVector(float x, float y);
		virtual float getMoveVectorX() const;
		virtual float getMoveVectorY() const;

		virtual float getCurrentSpeed();

	protected:

		Handle<IController> mController;

		float mMoveX;
		float mMoveY;

		bool mRunning;
		float mWalkingSpeed;
		float mRunningSpeed;
		float mAcceleration;
		float mCurrentSpeed;

		// TODO: Probably need to be a vector, or even a different class that
		// keeps track of multiple animations.
		Handle<Sprite> mGraphic;
		Handle<TextField> mInfo;

		string mName;
	};

}
}
