#pragma once

#include <string>
#include <vector>
#include "json_value.h"
#include "ifont.h"

#include "igl_gfx_component.h"

using namespace std;
using namespace am::util;

namespace am {
namespace gfx {

	class GlTexture;
	class GlAsset;
	class GlGfxEngine;

	class GlFont : public IFont, public IGlGfxComponent {
	public:
		// GlFont methods
		~GlFont();

		GlAsset *getGlAsset();
		void setGlAsset(GlAsset *asset);

		// IFont methods
		virtual IAsset *getAsset();
		virtual void setAsset(IAsset *asset);

		virtual string getName() const;

		virtual int loadDef(JsonValue value);

		virtual void getTextureWindow(char ch, TextureWindow &render);

		virtual bool isLoaded() const;
		virtual bool isFixedWidth() const;

		virtual float getCharHeight() const;
		virtual float getFixedCharWidth() const;

		virtual void setKerning(float kerning);
		virtual float getKerning() const;

		virtual void setLeading(float leading);
		virtual float getLeading() const;

		virtual void setSpaceWidth(float width);
		virtual float getSpaceWidth() const;

		virtual void setTabWidth(float width);
		virtual float getTabWidth() const;

		virtual void measureText(const char *text, float &width, float &height) const;
		virtual void measureWord(const char *word, float &width, float &height) const;

		virtual float getVariableTabPosition(float xPos) const;

		virtual void setCharsAcross(int across);
		virtual int getCharsAcross() const;
		virtual int getCharsDown() const;

		// IGfxComponent methods
		virtual IGfxEngine *getGfxEngine();
		// IGlGfxComponent methods
		virtual GlGfxEngine *getGlGfxEngine();

		friend class GlGfxEngine;

	protected:

		GlFont(GlGfxEngine *engine, const char *name);

		GlAsset *mAsset;
		GlGfxEngine *mGfxEngine;
		string mName;

		bool mFixedWidth;
		bool mUtfSupport;

		float mCharHeight;
		float mFixedCharWidth;

		float mKerning;
		float mLeading;
		float mSpaceWidth;
		float mTabWidth;

		int mCharsAcross;
		int mCharsDown;

		vector<TextureWindow> mTextureWindows;

		pair<float, float> processChar(int *data, int xPos, int yPos);
		void postLoad();

	};

}
}
