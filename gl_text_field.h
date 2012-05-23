#pragma once

#include "itext_field.h"
#include "igl_renderable.h"
#include "texture_window.h"

#include "transform.h"

namespace am {
namespace gfx {

	class IGfxEngine;
	class IFont;

	class GlGfxEngine;
	class GlFont;

	class GlTextField : public ITextField, public IGlRenderable {
	public:
		// GlTextField methods
		GlTextField(GlGfxEngine *engine);
		~GlTextField();

		GlFont *getBaseGlFont();
		void setBaseGlFont(GlFont *font);

		float getRenderedHeight() const;

		// ITextField methods
		virtual IFont *getBaseFont();
		virtual void setBaseFont(IFont *font);
		
		virtual void setText(string &str);
		virtual void appendText(string &str);
		virtual string getText();

		virtual float getMeasuredWidth();
		virtual float getMeasuredHeight();

		// IRenderable methods
		virtual void render(float dt);
		virtual am::math::Transform &getTransform();

		virtual void setWidth(float width);
		virtual float getWidth() const;

		virtual void setHeight(float height);
		virtual float getHeight() const;

		// IGfxComponent methods
		virtual IGfxEngine *getGfxEngine();
		// IGlGfxComponent methods
		virtual GlGfxEngine *getGlGfxEngine();

		friend class GlGfxEngine;

	protected:

		string mText;
		GlFont *mFont;

		float mMeasuredWidth;
		float mMeasuredHeight;

		float mRenderedHeight;

		float mCurrXpos;
		float mCurrYpos;

		bool mInWord;

		bool mDirty;

		TextureWindow mCharRender;

		am::math::Transform mTransform;
		float mWidth;
		float mHeight;
		GlGfxEngine *mGfxEngine;

		void calcSize();

		void preRender();
		void postRender();
		void newLine();
		void renderText(const string &text);
	};

}
}
