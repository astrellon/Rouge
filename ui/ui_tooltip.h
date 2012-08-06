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
namespace ui {

	class Tooltip : public Layer {
	public:
		Tooltip();
		Tooltip(const char *tooltip, const char *detailed = NULL, Renderable *target = NULL);
		~Tooltip();

		enum TooltipState {
			HIDDEN, ACTIVE, VISIBLE, VISIBLE_DETAILED
		};

		virtual void setText(const char *text);
		virtual const char *getText() const;

		virtual void setDetailedText(const char *text);
		virtual const char *getDetailedText() const;

		virtual void active(Renderable *target = NULL);
		virtual void show(Renderable *target = NULL);
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

		virtual Sprite *getGraphic();
		virtual TextField2 *getTextField();
		virtual TextField2 *getDetailedTextField();

		virtual void render(float dt);

	protected:

		TooltipState mState;

		float mTimeCounter;
		float mDisplayDelay;
		float mDetailedDisplayDelay;

		float mTextWidth;
		float mTextHeight;

		float mDetailedTextWidth;
		float mDetailedTextHeight;

		Handle<Renderable> mTarget;
		Handle<Sprite> mGraphic;
		Handle<TextField2> mTextField;
		Handle<TextField2> mDetailedTextField;

		virtual void setDetailed(bool detailed);
	};

}
}
