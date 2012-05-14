#include "main.h"

// Navigation related
int oldMouseButton = 0, oldX = -1, oldY = -1;
bool mouseLeftDown = false;
bool mouseRightDown = false;
float zoom = 90;
float camRotateX = 0.0f, camRotateY = 0.0f;

// Should an error occur during initialization, such as SSE not being
// supported during execution of an SSE build. This will be set to true
// and the program will only display an error message.
bool displayError = false;

// Rendering related
int screenWidth = 600, screenHeight = 400;
//TextField txtInstructions;

float g_cameraPosition[3];
float g_lightDirection[3];

// Initializes OpenGL. Loads the vertex and fragment shaders, and creates the
// sphere mesh.
void glInit(void) 
{
	glClearColor (0.0, 0.3, 0.4, 0.0);
	// Enable texture for the text fields.
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);*/
}

am::ui::Font font;
am::ui::TextField textField;

void init()
{
	// Initialize the DevIL framework.
	ilInit();

	// Initialize OpenGL
	glInit();

	am::util::JsonValue loaded = am::util::JsonValue::import_from_file("data/fontDefs.ssff");
	font.loadDef(loaded["basic"]);

	textField.setFont(&font);
	textField.setText(string("Hello there Melli, how are you today?"));
}

// Setup the projection matrix for perspective.
// Used to render the scene.
void perspectiveView()
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	GLfloat zNear = 0.1f;
	GLfloat zFar = 1000.0f;
	GLfloat aspect = float(screenWidth)/float(screenHeight);
	GLfloat fH = tan( float(65.0f / 360.0f * 3.14159f) ) * zNear;
	GLfloat fW = fH * aspect;
	glFrustum( -fW, fW, -fH, fH, zNear, zFar );

	glMatrixMode(GL_MODELVIEW);
}

// Setup the projection matrix for orthographic.
// Used to render the interface.
void orthographicView()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(0, screenWidth, screenHeight, 0, 0, 1);
	
	glMatrixMode(GL_MODELVIEW);
}

// Called before every render.
void update(DWORD milli)
{
	if(displayError)
		return;

	static DWORD time = 0;
}

void display(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	orthographicView();
	textField.render(0, 0);

	glFlush();

	return;

	glPushMatrix();

	// Setup the navigation
	glTranslated(0, 0, -zoom);
	glRotatef(camRotateX, 1, 0, 0);
	glRotatef(camRotateY, 0, 1, 0);
	//glTranslated(0, ROOM_SIZE, 0);

	// Setup drawing for the scene.
	perspectiveView();
	glFlush();
}

void reshape (int w, int h)
{
	// Screen size is used by both the perspective and orthographic view functions.
	screenWidth = w;
	screenHeight = h;
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
}

void mouseFunc(int mouseButton, int x, int y)
{
	// If we are pressing a mouse button that was different to the last
	// time we had a mouse move event, then we want to reset the oldX and oldY
	// coordinates.
	if(mouseButton != oldMouseButton)
	{
		oldX = x;
		oldY = y;
	}
	
	// If the user is pressing the left mouse button.
	if(mouseButton == 1)
	{
		int dx = x - oldX;
		int dy = y - oldY;

		camRotateX += (float)dy * 0.5f;
		camRotateY += (float)dx * 0.5f;
	}
	// If the user is pressing the right mouse button.
	else if(mouseButton == 2)
	{
		zoom += y - oldY;
	}

	oldX = x;
	oldY = y;

	oldMouseButton = mouseButton;
}

void keyboardFunc(BOOL *keys, int key)
{
	if(!keys[key])
	{
		// + key
		/*if(key == 107)
		{
			changeSphereCount(1000);
		}
		// - key
		else if(key == 109)
		{
			changeSphereCount(-1000);
		}

		// Change the diameter of the user sphere to be 
		// one more than the number of the key pressed.
		if(key >= '1' && key <= '9')
		{
			userSphereSize = (float)(key - '1' + 1);
		}*/
	}
}
/*
// Sets that the program should finish.
void deinit()
{
	programRunning = FALSE;
}

void terminateApplication (GL_Window* window)							// Terminate The Application
{
	PostMessage (window->hWnd, WM_QUIT, 0, 0);							// Send A WM_QUIT Message
	programRunning = FALSE;											// Stop Looping Of The Program
	
}*/

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	am::ui::WinWrap win;
	win.init(hInstance);
}