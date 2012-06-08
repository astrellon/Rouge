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

		void setAsset(Asset *asset);
		void setAsset(const char *assetName);
		Sprite *getGraphic();

	protected:

		am::util::Handle<Sprite> mGraphic;

	};

}
}
