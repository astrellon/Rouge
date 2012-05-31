#pragma once

#include "gfx_renderable.h"
#include "texture_window.h"

#include "../transform.h"

namespace am {
namespace gfx {

	class GfxEngine;
	class Font;

	class TextField : public Renderable {
	public:
		// GlTextField methods
		TextField(GfxEngine *engine);
		~TextField();

		float getRenderedHeight() const;

		Font *getBaseFont();
		void setBaseFont(Font *font);
		
		void setText(string &str);
		void appendText(string &str);
		string getText();

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

		TextureWindow mCharRender;

		void calcSize();

		void preRender();
		void postRender();
		void newLine();
		void renderText(const string &text);
	};

}
}
