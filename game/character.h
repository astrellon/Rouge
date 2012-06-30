#pragma once

#include <base/handle.h>
using namespace am::base;

#include <string>
using namespace std;

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_sprite.h>
using namespace am::gfx;

#include "icontroller.h"
#include "game_object.h"

namespace am {
namespace game {

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

		virtual void setController(IController *controller);
		virtual IController *getController();

		virtual void setMoveVector(int x, int y);
		virtual int getMoveVectorX() const;
		virtual int getMoveVectorY() const;

	protected:

		Handle<IController> mController;

		int mMoveX;
		int mMoveY;

		// TODO: Probably need to be a vector, or even a different class that
		// keeps track of multiple animations.
		Handle<Sprite> mGraphic;
		Handle<TextField> mInfo;

		string mName;
	};

}
}
