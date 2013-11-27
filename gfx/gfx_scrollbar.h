#pragma once

#include <base/handle.h>
using namespace am::base;

#include <vector>
using namespace std;

#include <ui/ievent_listener.h>
using am::ui::IEventListener;

#include "gfx_layer.h"
#include "gfx_sprite.h"
#include "gfx_button.h"

namespace am {
namespace gfx {

	class Asset;

	class Scrollbar : public Layer, public IEventListener {
	public:
		Scrollbar(const char *btnUpAsset, const char *btnDownAsset, const char *barAsset, const char *backAsset);
		~Scrollbar();

		virtual void onEvent(Event *e);
		virtual void onEvent(MouseEvent *e);

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual int getMinValue() const;
		virtual void setMinValue(int value);

		virtual int getMaxValue() const;
		virtual void setMaxValue(int value);

		virtual int getValue() const;
		virtual void setValue(int value);

		virtual void render(float dt);

		static const char *SCROLL_VALUE_CHANGE;

	protected:

		Handle<Button> mBtnUp;
		Handle<Button> mBtnDown;
		Handle<Sprite> mBack;
		Handle<Button> mBar;

		int mMinValue;
		int mMaxValue;
		int mValue;

		void updateBar();

	};

}
}
