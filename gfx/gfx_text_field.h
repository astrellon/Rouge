#pragma once

#include "gfx_renderable.h"
#include "texture_window.h"

#include "../transform.h"

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
		const string &getText() const;
		string getText();

		void setAlignment(TextAlignment align);
		TextAlignment getAlignment() const;

		float getMeasuredWidth();
		float getMeasuredHeight();

		// Renderable methods
		virtual void render(float dt);
		
	protected:

		string mText;
		Font *mFont;

		float mMeasuredWidth;
		float mMeasuredHeight;

		float mRenderedHeight;

		float mCurrXpos;
		float mCurrYpos;

		bool mInWord;

		bool mDirty;

		TextAlignment mAlignment;

		TextureWindow mCharRender;

		void calcSize();

		virtual void preRender(float dt);
		virtual void postRender(float dt);
		void newLine();
		void renderText(const string &text);
		void checkAlignment(const char *line);
	};

}
}
