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

		void setValues(float topY, float bottomY,
			float leftX, float rightX);

		void setValues(float width, float height,
			float topY, float bottomY,
			float leftX, float rightX);

		void createSubWindow(TextureWindow &sub) const;

		friend class IFont;
		friend class Font;
		friend class Asset;

	protected:
		// Represents the size in pixels
		float mWidth;
		float mHeight;

		// Represents the UV coordinates.
		float mTopY;
		float mBottomY;
		float mLeftX;
		float mRightX;

	};

}
}
