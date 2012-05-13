#pragma once

#include <string>
#include <map>
#include <vector>
#include "json_value.h"

using namespace std;
using namespace am::util;

namespace am {
namespace base {

	class Texture;
}

namespace ui {

	class CharRender {
	public:
		CharRender();
		~CharRender();

		float getWidth() const;
		float getHeight() const;

		float getTopY() const;
		float getBottomY() const;

		float getLeftX() const;
		float getRightX() const;

		friend class Font;

	protected:
		float mWidth;
		float mHeight;

		float mTopY;
		float mBottomY;
		float mLeftX;
		float mRightX;

	};

	class Font {
	public:
		Font();
		~Font();

		am::base::Texture *getTexture();
		void setTexture(am::base::Texture *texture);

		int loadDef(JsonValue value);

		void getCharRender(char ch, CharRender &render);

		bool isLoaded() const;

	protected:
		am::base::Texture *mTexture;
		bool mFixedWidth;
		bool mUtfSupport;

		float mCharHeight;
		float mFixedCharWidth;

		vector<CharRender> mCharRenders;

		pair<float, float> processChar(int *data, int xPos, int yPos);
		void postLoad();

	};

}
}
