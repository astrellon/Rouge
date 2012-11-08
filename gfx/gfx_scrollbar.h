#pragma once

#include <base\handle.h>
using namespace am::base;

#include <vector>
using namespace std;

#include "gfx_layer.h"
#include "gfx_sprite.h"

namespace am {
namespace gfx {

	class Asset;

	class Scrollbar : public Layer {
	public:
		Scrollbar(const char *btnUpAsset, const char *btnDownAsset, const char *barAsset, const char *backAsset);
		~Scrollbar();

		//virtual float getWidth();
		virtual void setWidth(float width);

		//virtual float getHeight();
		virtual void setHeight(float height);

		virtual int getMinValue() const;
		virtual void setMinValue(int value);

		virtual int getMaxValue() const;
		virtual void setMaxValue(int value);

		virtual int getValue() const;
		virtual void setValue(int value);

	protected:

		Handle<Sprite> mBtnUp;
		Handle<Sprite> mBtnDown;
		Handle<Sprite> mBack;
		Handle<Sprite> mBar;

		int mMinValue;
		int mMaxValue;
		int mValue;

		void updateBar();

	};

}
}
