#pragma once

#include "itext_field.h"
#include "igl_renderable.h"

#include "transform.h"

namespace am {
namespace gfx {

	class IGfxEngine;
	class IFont;

	class GlGfxEngine;
	class GlFont;

	class GlTextField : public ITextField, public IGlRenderable {
	public:
		GlTextField(GlGfxEngine *engine);
		~GlTextField();

		virtual IFont *getBaseFont();
		GlFont *getBaseGlFont();
		virtual void setBaseFont(IFont *font);
		void setBaseGlFont(GlFont *font);

		virtual void setText(string &str);
		virtual void appendText(string &str);
		virtual string getText();

		virtual void render(float dt);
		virtual am::math::Transform &getTransform();

		virtual IGfxEngine *getGfxEngine();
		virtual GlGfxEngine *getGlGfxEngine();

		friend class GlGfxEngine;

	protected:

		string mText;
		GlFont *mFont;

		am::math::Transform mTransform;
		GlGfxEngine *mGfxEngine;
	};

}
}
