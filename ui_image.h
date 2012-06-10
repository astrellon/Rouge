#pragma once

#include "ui_component.h"
#include "handle.h"

namespace am {
namespace gfx {
	class GfxEngine;
	class Sprite;
	class Asset;
}

using namespace am::gfx;

namespace ui {

	class Image : public UIComponent {
	public:
		Image(GfxEngine *engine);
		Image(GfxEngine *engine, Asset *asset);
		Image(GfxEngine *engine, const char *assetName);
		~Image();

		virtual Colour &getColour();
		virtual void setColour(const Colour &colour);
		virtual void setColour(float red, float green, float blue);
		virtual void setColour(float red, float green, float blue, float alpha);
		virtual void setAlpha(float alpha);
		virtual float getAlpha() const;

		void setAsset(Asset *asset);
		void setAsset(const char *assetName);
		Sprite *getGraphic();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

	protected:

		am::util::Handle<Sprite> mGraphic;

	};

}
}
