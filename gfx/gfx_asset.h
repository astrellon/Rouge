#pragma once

#include <string>
#include <vector>
using namespace std;

#include <base/handle.h>
#include <base/imanaged.h>

#include <lua/lua_state.h>
using namespace am::lua;

#include <util/idefinition.h>
using namespace am::util;

#include "scale_nine.h"
#include "texture_window.h"

namespace am {
namespace gfx {

	class Texture;

	class Asset : public am::base::IManaged, public IDefinition {
	public:

		typedef vector<TextureWindow> AnimationWindows;
		typedef vector< am::base::Handle<Texture> > TextureList;
		
		// GlAsset methods
		Asset(const char *name);
		~Asset();

		const char *getName() const;

		void setLoadedName(const char *loadedName);
		const char *getLoadedName() const;

		void setTexture(Texture *texture, unsigned int index = 0);
		Texture *getTexture(unsigned int index = 0) const;

		void addTexture(Texture *texture);
		void removeTexture(const char *filename);
		void removeTexture(Texture *texture);
		bool hasTexture(const Texture *texture) const;
		bool hasTexture(const char *filename) const;
		int getTotalTextures() const;
		const TextureList &getAllTextures() const;

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

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		string mName;
		TextureList mTextures;
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
