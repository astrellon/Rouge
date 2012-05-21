#pragma once

namespace am {
namespace gfx {

	class TextureWindow {
	public:
		TextureWindow();
		~TextureWindow();

		float getWidth() const;
		float getHeight() const;

		float getTopY() const;
		float getBottomY() const;

		float getLeftX() const;
		float getRightX() const;

		void setValues(float width, float height,
			float topY, float bottomY,
			float leftX, float rightX);

		friend class IFont;
		friend class GlFont;

	protected:
		float mWidth;
		float mHeight;

		float mTopY;
		float mBottomY;
		float mLeftX;
		float mRightX;

	};

}
}
