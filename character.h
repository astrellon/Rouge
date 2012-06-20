#pragma once

#include "game_object.h"
#include "handle.h"
using namespace am::util;

#include <string>
using namespace std;

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

		virtual void setController(IController *controller);
		virtual IController *getController();

	protected:

		Handle<IController> mController;

		// TODO: Probably need to be a vector, or even a different class that
		// keeps track of multiple animations.
		Handle<Sprite> mGraphic;

		string mName;
	};

}
}
