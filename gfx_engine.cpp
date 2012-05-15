#include "gfx_engine.h"

#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "IL/il.h"

#include <math.h>

namespace am {
namespace ui {
	GfxEngine::GfxEngine()
	{

	}
	GfxEngine::~GfxEngine()
	{

	}

	void GfxEngine::init()
	{
		// Initialize the DevIL framework.
		ilInit();

		// Initialize OpenGL
		//glInit();

		glClearColor (0.0, 0.3, 0.4, 0.0);
		// Enable texture for the text fields.
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
	
		/*glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);*/
	}
	void GfxEngine::deinit()
	{
		ilShutDown();
	}
	void GfxEngine::setOrthographic()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0, mScreenWidth, mScreenHeight, 0, 0, 1);
	
		glMatrixMode(GL_MODELVIEW);
	}
	void GfxEngine::setPerspective()
	{
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity();

		GLfloat zNear = 0.1f;
		GLfloat zFar = 1000.0f;
		GLfloat aspect = float(mScreenWidth)/float(mScreenHeight);
		GLfloat fH = tan( float(65.0f / 360.0f * 3.14159f) ) * zNear;
		GLfloat fW = fH * aspect;
		glFrustum( -fW, fW, -fH, fH, zNear, zFar );

		glMatrixMode(GL_MODELVIEW);
	}

	void GfxEngine::display()
	{
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
	}

	void GfxEngine::reshape(int width, int height)
	{
		mScreenWidth = width;
		mScreenHeight = height;
		glViewport (0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	}
}
}
