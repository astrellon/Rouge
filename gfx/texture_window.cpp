#include "texture_window.h"

namespace am {
namespace gfx {

	TextureWindow::TextureWindow() :
		mWidth(0.0f),
		mHeight(0.0f),
		mLeftX(0.0f),
		mRightX(1.0f),
		mTopY(0.0f),
		mBottomY(1.0f)
	{
	}
	TextureWindow::~TextureWindow()
	{
	}

	float TextureWindow::getWidth() const
	{
		return mWidth;
	}
	float TextureWindow::getHeight() const
	{
		return mHeight;
	}

	float TextureWindow::getTopY() const
	{
		return mTopY;
	}
	float TextureWindow::getBottomY() const
	{
		return mBottomY;
	}

	float TextureWindow::getLeftX() const
	{
		return mLeftX;
	}
	float TextureWindow::getRightX() const
	{
		return mRightX;
	}

	void TextureWindow::createSubWindow(TextureWindow &sub) const
	{
		float uvWidth = mRightX - mLeftX;
		float uvHeight = mBottomY - mTopY;

		sub.mLeftX = sub.mLeftX * uvWidth + mLeftX;
		sub.mRightX = sub.mRightX * uvWidth + mLeftX;
		sub.mTopY = sub.mTopY * uvHeight + mTopY;
		sub.mBottomY = sub.mBottomY * uvHeight + mTopY;

		sub.mWidth *= uvWidth;
		sub.mHeight *= uvHeight;
	}

	void TextureWindow::setValues(float topY, float bottomY, float leftX, float rightX)
	{
		mWidth = -1.0f;
		mHeight = -1.0f;
		mTopY = topY;
		mBottomY = bottomY;
		mLeftX = leftX;
		mRightX = rightX;
	}

	void TextureWindow::setValues(float width, float height,
		float topY, float bottomY,
		float leftX, float rightX)
	{
		mWidth = width;
		mHeight = height;
		mTopY = topY;
		mBottomY = bottomY;
		mLeftX = leftX;
		mRightX = rightX;
	}
}
}
