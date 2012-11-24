#pragma once

#include <base/handle.h>
using namespace am::base;

#include "ui_component.h"

namespace am {
namespace gfx {
	class Sprite;
	class Asset;
}

using namespace am::gfx;

namespace ui {

	class Image : public UIComponent {
	public:
		Image();
		Image(Asset *asset);
		Image(const char *assetName);
		~Image();

		/*virtual Colour &getColour();
		virtual void setColour(const Colour &colour);
		virtual void setColour(float red, float green, float blue);
		virtual void setColour(float red, float green, float blue, float alpha);
		virtual void setAlpha(float alpha);
		virtual float getAlpha() const;*/

		virtual void setGfxComponent(GfxComponent *comp);
		virtual GfxComponent *getGfxComponent() const;

		void setAsset(Asset *asset);
		void setAsset(const char *assetName);
		Sprite *getGraphic();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

	protected:

		Handle<Sprite> mGraphic;

	};

}
}
