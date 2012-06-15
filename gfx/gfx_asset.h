#pragma once

#include "gfx_component.h"
#include "texture_window.h"
#include "../json_value.h"
#include <string>

#include "scale_nine.h"

#include "../handle.h"
#include "../imanaged.h"

using namespace std;
using namespace am::util;

namespace am {
namespace gfx {

	class Texture;

	class Asset : public IManaged {
	public:

		typedef vector<TextureWindow> AnimationWindows;
		
		// GlAsset methods
		Asset(const char *name);
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

		void setScaleNine(const ScaleNine &scaleNine);
		void setScaleNine(const ScaleNine &scaleNine, ScaleNineState state);
		ScaleNine &getScaleNine();

		void setScaleNineState(ScaleNineState state);
		ScaleNineState getScaleNineState() const;

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
		ScaleNine mScaleNine;
		ScaleNineState mScaleNineState;

		AnimationWindows mAnimationWindows;
	};

}
}
