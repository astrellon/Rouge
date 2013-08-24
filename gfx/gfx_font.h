#pragma once

#include <string>
#include <vector>
using namespace std;

#include <lua/lua_state.h>
using namespace am::lua;

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

#include <util/idefinition.h>
using namespace am::util;

#include "texture_window.h"

namespace am {
namespace gfx {

	class Texture;
	class Asset;

	class Font : public IManaged, public IDefinition {
	public:
		// GlFont methods
		Font(const char *name);
		~Font();

		Asset *getAsset();
		void setAsset(Asset *asset);

		string getName() const;

		void getTextureWindow(unsigned char ch, TextureWindow &render);

		bool isLoaded() const;

		void setFixedWidth(bool fixed);
		bool isFixedWidth() const;

		float getCharHeight() const;
		float getFixedCharWidth() const;

		void setKerning(float kerning);
		float getKerning() const;

		void setLeading(float leading);
		float getLeading() const;

		void setSpaceWidth(float width);
		float getSpaceWidth() const;

		void setTabWidth(float width);
		float getTabWidth() const;

		void setUtf(bool utf);
		bool isUtf() const;

		int measureText(const char *text, float containerWidth, float &width, float &height, char endChar = '\0') const;
		int measureLine(const char *text, float containerWidth, float &width, float &height) const;
		int measureWord(const char *word, float &width, float &height) const;

		float getVariableTabPosition(float xPos) const;

		void setCharsAcross(int across);
		int getCharsAcross() const;
		int getCharsDown() const;
		float getCharWidth(unsigned char ch) const;

		void process();

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		Handle<Asset> mAsset;
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
		
	};

}
}
