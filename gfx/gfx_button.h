#pragma once

#include <base\handle.h>
using namespace am::base;

#include <vector>
using namespace std;

#include <ui/ievent_listener.h>
using am::ui::IEventListener;

#include "gfx_sprite.h"
#include "gfx_renderable.h"

namespace am {
namespace gfx {

	class Button : public Sprite, public IEventListener {
	public:
		Button(Renderable *hitbox = NULL);
		Button(const char *assetName, Renderable *hitbox = NULL);
		~Button();

		void setHitbox(Renderable *hitbox);
		Renderable *getHitbox();

		virtual void setEnabled(bool enabled);
		virtual bool isEnabled() const;

		virtual void onEvent(MouseEvent *e);
		
	protected:

		Handle<Renderable> mHitbox;
		bool mEnabled;

		void addListeners(Renderable *target);
		void removeListeners(Renderable *target);

	};

}
}
