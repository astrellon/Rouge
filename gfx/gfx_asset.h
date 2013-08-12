#pragma once

#include <util/json_value.h>
using namespace am::util;

#include <string>
#include <vector>
using namespace std;

#include <base/handle.h>
#include <base/imanaged.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include "scale_nine.h"
#include "texture_window.h"

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

		void setTexture(const Texture *texture, unsigned int index = 0);
		const Texture *getTexture(unsigned int index = 0) const;

		void addTexture(const Texture *texture);
		void removeTexture(const Texture *texture);
		bool hasTexture(const Texture *texture) const;
		int getTotalTextures() const;

		void setTextureWindow(const TextureWindow &window);
		const TextureWindow &getTextureWindow() const;

		void setNumFramesX(int num);
		int getNumFramesX() const;
		
		void setNumFramesY(int num);
		int getNumFramesY() const;

		void setTotalSubWindows(int frames);
		int getTotalSubWindows() const;

		void setFrameRate(float rate);
		float getFrameRate() const;

		void setSubWindowFrames(int numX, int numY, int totalFrames, float frameRate);
		void setSubWindowAnimation(bool animateSubWindow);
		bool isSubWindowAnimation() const;

		const AnimationWindows &getAnimationWindows() const;
		void processAnimation();

		float getWidth();
		float getHeight();

		// The table to load the definition off needs to be on top of the stack.
		int loadDef(LuaState &lua, bool reload = false);
		void assign(const Asset &rhs);

		void setRepeatX(bool repeat);
		bool getRepeatX() const;

		void setRepeatY(bool repeat);
		bool getRepeatY() const;

		void setScaleNine(const ScaleNine &scaleNine);
		void setScaleNine(const ScaleNine &scaleNine, ScaleNineState state);
		ScaleNine &getScaleNine();

		void setScaleNineState(ScaleNineState state);
		ScaleNineState getScaleNineState() const;

	protected:

		string mName;
		vector<const Texture *> mTextures;
		TextureWindow mWindow;

		bool mAnimateSubWindows;

		int mNumFramesX;
		int mNumFramesY;
		int mTotalSubWindows;
		float mFrameRate;

		float mWidth;
		float mHeight;

		bool mRepeatX;
		bool mRepeatY;
		bool mAnimationDirty;
		ScaleNine mScaleNine;
		ScaleNineState mScaleNineState;

		AnimationWindows mAnimationWindows;
	};

}
}
