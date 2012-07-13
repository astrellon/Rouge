#pragma once

#include <vector>
using namespace std;

#include "gfx_renderable.h"
#include "texture_window.h"
#include "gfx_text_style.h"

namespace am {
namespace gfx {

	class Font;

	class TextField : public Renderable {
	public:
		// GlTextField methods
		enum TextAlignment {
			ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER
		};

		TextField();
		~TextField();

		float getRenderedHeight() const;

		Font *getBaseFont();
		void setBaseFont(Font *font);
		void setBaseFont(const char *fontName);
		
		void setText(const char *str);
		void setText(const string &str);
		void appendText(const char *str);
		void appendText(const string &str);
		const char *getText();
		const char *getRawText() const;

		void setAlignment(TextAlignment align);
		TextAlignment getAlignment() const;

		float getMeasuredWidth();
		float getMeasuredHeight();

		void clearAllStyles();
		void addTextStyle(const TextStyle &style);
		void removeStyleAt(int index);
		int getNumStyles() const;
		bool getStyleAt(int index, TextStyle &style);

		// Renderable methods
		virtual void render(float dt);

		virtual void setWidth(float width);
		virtual void setHeight(float height);
		
	protected:

		string mText;
		string mRawText;
		Font *mFont;

		float mMeasuredWidth;
		float mMeasuredHeight;

		float mRenderedHeight;

		float mCurrXpos;
		float mCurrYpos;

		bool mInWord;
		bool mDirty;
		bool mTextDirty;

		int mTextPosition;
		int mStylePosition;

		typedef vector<TextStyle> TextStyleList;
		TextStyleList mStyles;

		typedef vector<Colour> ColourList;
		ColourList mColourStack;

		TextAlignment mAlignment;
		TextureWindow mCharRender;

		void calcSize();

		virtual void preRender(float dt);
		virtual void postRender(float dt);
		void newLine();
		void renderText(const string &text);
		void checkAlignment(const char *line);

		void parseRawText();
	};

}
}
