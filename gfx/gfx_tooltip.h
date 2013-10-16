#pragma once

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_text_field2.h>
#include <gfx/gfx_sprite.h>
#include <gfx/gfx_layer.h>
using namespace am::gfx;

#include <string>
using namespace std;

namespace am {
namespace gfx {

	class Tooltip : public Layer, public IEventListener {
	public:
		Tooltip();
		Tooltip(Renderable *target);
		Tooltip(const Tooltip &copy);
		~Tooltip();

		virtual Renderable *clone() const;

		enum TooltipState {
			HIDDEN, ACTIVE, VISIBLE, VISIBLE_DETAILED
		};

		virtual void active(Renderable *target = nullptr);
		virtual void show(Renderable *target = nullptr);
		virtual void hide();

		virtual void setTarget(Renderable *target);
		virtual Renderable *getTarget() const;

		virtual void setState(TooltipState state);
		virtual TooltipState getState() const;

		virtual void setDisplayDelay(float delay);
		virtual float getDisplayDelay() const;

		virtual void setDetailedDisplayDelay(float delay);
		virtual float getDetailedDisplayDelay() const;

		virtual void setDelays(float displayDelay, float detailedDelay);

		virtual float getTimeCounter() const;

		virtual Sprite *getGraphic() const;
		virtual void setTooltipGraphic(Renderable *graphic);
		virtual Renderable *getTooltipGraphic() const;
		virtual void setTooltipDetailedGraphic(Renderable *graphic);
		virtual Renderable *getTooltipDetailedGraphic() const;

		virtual void render(float dt);

		virtual void onEvent(MouseEvent *e);

	protected:

		TooltipState mState;

		float mTimeCounter;
		float mDisplayDelay;
		float mDetailedDisplayDelay;

		Handle<Renderable> mTarget;
		Handle<Sprite> mGraphic;
		Handle<Renderable> mTooltipGraphic;
		Handle<Renderable> mTooltipDetailedGraphic;

		virtual void setDetailed(bool detailed);
	};

}
}
