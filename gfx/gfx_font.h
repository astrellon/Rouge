#pragma once

#include <string>
#include <vector>
#include "../json_value.h"

#include "../imanaged.h"

#include "texture_window.h"

using namespace std;
using namespace am::util;

namespace am {
namespace gfx {

	class Texture;
	class Asset;

	class Font : public IManaged {
	public:
		// GlFont methods
		Font(const char *name);
		~Font();

		Asset *getAsset();
		void setAsset(Asset *asset);

		string getName() const;

		int loadDef(JsonValue value);

		void getTextureWindow(unsigned char ch, TextureWindow &render);

		bool isLoaded() const;
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

		int measureText(const char *text, float containerWidth, float &width, float &height, char endChar = '\0') const;
		int measureLine(const char *text, float containerWidth, float &width, float &height) const;
		int measureWord(const char *word, float &width, float &height) const;

		float getVariableTabPosition(float xPos) const;

		void setCharsAcross(int across);
		int getCharsAcross() const;
		int getCharsDown() const;

	protected:

		Asset *mAsset;
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
