#pragma once

#include "gfx_sprite.h"
#include "texture_window.h"
#include "scale_nine.h"

namespace am {
namespace gfx {

	class GfxEngine;
	class Asset;

	class SpriteScaleNine : public Sprite {
	public:
		SpriteScaleNine(GfxEngine *engine);
		SpriteScaleNine(GfxEngine *engine, Asset *asset);
		SpriteScaleNine(GfxEngine *engine, const char *assetName);
		~SpriteScaleNine();

		void setScaleNine(const ScaleNine &scaleNine);
		void setScaleNine(const ScaleNine &scaleNine, bool enable);
		ScaleNine &getScaleNine();

		void setScaleNineEnabled(bool enable);
		bool isScaleNineEnabled() const;

	protected:

		ScaleNine mScaleNine;
		bool mEnableScaleNine;

		virtual void renderTexture(const TextureWindow &win, const float &width, const float &height);
	};

}
}
