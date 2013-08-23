#pragma once

#include <vector>
using namespace std;

#include <ui/ievent_listener.h>
#include <ui/mouse_event.h>
using namespace am::ui;

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
		TextField(const TextField &rhs);
		~TextField();

		virtual Renderable *clone() const;

		float getRenderedHeight() const;

		Font *getBaseFont();
		void setBaseFont(Font *font);
		void setBaseFont(const char *fontName);
		
		void setText(const char *str);
		void setText(const string &str);
		void appendText(const char *str);
		void appendText(const string &str);
		const char *getText() const;
		int length() const;

		void setAlignment(TextAlignment align);
		TextAlignment getAlignment() const;

		float getMeasuredWidth();
		float getMeasuredHeight();

		int getTextPosition(int localX, int localY) const;

		void setCursorInputPosition(int position);
		int getCursorInputPosition() const;

		int getLineAbove(int textPosition) const;
		int getLineBelow(int textPosition) const;
		int getStartOfLine(int textPosition) const;
		int getEndOfLine(int textPosition) const;

		// Renderable methods
		virtual void render(float dt);

		virtual void setWidth(float width);
		virtual void setHeight(float height);
		
	protected:

		string mText;
		Handle<Font> mFont;

		float mMeasuredWidth;
		float mMeasuredHeight;

		float mRenderedHeight;

		float mCurrXpos;
		float mCurrYpos;

		float mCursorXpos;
		float mCursorYpos;

		bool mInWord;
		bool mDirty;
		bool mNewLineDirty;

		int mCursorInputPosition;
		int mTextPosition;
		
		typedef vector<int> NewLineList;
		NewLineList mNewLinePositions;

		TextAlignment mAlignment;
		TextureWindow mCharRender;

		void calcSize();

		virtual void preRender(float dt);
		virtual void postRender(float dt);
		void newLine();
		void renderText(const string &text);
		void checkAlignment(const char *line);

		void checkCursorInput();
		void drawCursorInput();

		int getLineOfText(int textPosition) const;
	};

}
}
