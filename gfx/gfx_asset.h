#pragma once

#include "gfx_component.h"
#include "texture_window.h"
#include "../json_value.h"
#include <string>

using namespace std;
using namespace am::util;

namespace am {
namespace gfx {

	class Texture;
	class GfxEngine;

	class Asset : public GfxComponent {
	public:

		typedef vector<TextureWindow> AnimationWindows;
		
		// GlAsset methods
		Asset(GfxEngine *engine, const char *name);
		~Asset();

		const char *getName() const;

		void setTexture(const Texture *texture);
		const Texture *getTexture() const;

		void setTextureWindow(const TextureWindow &window);
		const TextureWindow &getTextureWindow() const;

		void setNumFramesX(int num);
		int getNumFramesX() const;
		
		void setNumFramesY(int num);
		int getNumFramesY() const;

		void setTotalFrames(int frames);
		int getTotalFrames() const;

		void setFrameRate(float rate);
		float getFrameRate() const;

		void setAnimation(int numX, int numY, int totalFrames, float frameRate);

		const AnimationWindows &getAnimationWindows() const;
		void processAnimation();

		float getWidth();
		float getHeight();

		int loadDef(JsonValue value);

	protected:

		string mName;
		const Texture *mTexture;
		TextureWindow mWindow;

		int mNumFramesX;
		int mNumFramesY;
		int mTotalFrames;
		float mFrameRate;

		float mWidth;
		float mHeight;

		bool mAnimationDirty;

		AnimationWindows mAnimationWindows;
	};

}
}
