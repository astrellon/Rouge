#pragma once

#include <base/handle.h>

#include "ui_component.h"

namespace am {
namespace gfx {
	class Sprite;
	class Asset;
}

namespace ui {

	class Image : public UIComponent {
	public:
		Image();
		Image(gfx::Asset *asset);
		Image(const char *assetName);
		~Image();

		virtual void setGfxComponent(gfx::GfxComponent *comp);
		virtual gfx::GfxComponent *getGfxComponent() const;

		void setAsset(gfx::Asset *asset);
		void setAsset(const char *assetName);
		gfx::Sprite *getGraphic();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

	protected:

		base::Handle<gfx::Sprite> mGraphic;

	};

}
}
