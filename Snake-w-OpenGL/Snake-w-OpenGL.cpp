#include <Windows.h>
#include <conio.h>
#include <ctime>

//for putting text on the screen
#include <stdio.h>
#include <stdarg.h>

#include <GL/GL.h>
#include <GL/GLU.h>
#include <glut.h>

#include "World.h"
#include "ScreenBuffer.h"
#include "Snake.h"

HGLRC		hRC = NULL; //permanent rendering context
			//every openGL program linked to a rendering context
HDC			hDC = NULL;	//private GDI device context
			//to draw the program to a window
HWND		hWnd = NULL; // holds windows handle
HINSTANCE	hInstance;  //holds the instance of app

ScreenBuffer buffer;
World w;

int player_count = 1;

bool keys[256];  //array that helps monitor key presses
bool active = TRUE; //is active on desktop or minimized
bool fullscreen = TRUE;  //im not that dumb

//holds the number of the display list that will be created
GLuint base;
//two counters to adjust position and coloring
GLfloat cnt1;
GLfloat cnt2;

GLfloat game_height = 600;
GLfloat game_width = 800;

//declaration of WndProc
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//building the actual font
GLvoid BuildFont(GLvoid)
{
	HFONT font;  //windows font id
	HFONT oldfont;  //used for good house keeping

	base = glGenLists(96);  //storaging 96 chars

	font = CreateFont(64,  //height of the font
		24,  //width of the font
		0,  //angle of escapement
		0,  //orientation angle
		FW_BOLD,  //font weight
		FALSE,  //italic
		FALSE,  //underline
		FALSE,  //strikeout
		ANSI_CHARSET,  //char set indentifier
		OUT_TT_PRECIS,  //output precision
		CLIP_DEFAULT_PRECIS,  //clipping precision
		ANTIALIASED_QUALITY,  //output quality
		FF_DONTCARE | DEFAULT_PITCH,  //family and pitch
		"Galahad Std Regular");  //font name

	oldfont = (HFONT)SelectObject(hDC, font);  //selecting wanted font
	wglUseFontBitmaps(hDC, 32, 96, base);  //builds 96 chars, starting at char 32
	SelectObject(hDC, oldfont);  //selecting wanted font
	DeleteObject(font);  //delete font
}

//deleting all 96 displays
GLvoid KillFont(GLvoid)
{
	glDeleteLists(base, 96);
}

GLvoid glPrint(const char *fmt, ...)
{
	char text[256];  //holds string
	va_list ap;  //pointer to list of arguments

	if (fmt == NULL) return;

	va_start(ap, fmt);
	vsprintf_s(text, fmt, ap);
	va_end(ap);

	glPushAttrib(GL_LIST_BIT);  //pushes the display list bits
	glListBase(base - 32);  //sets the base char to 32

	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);  //draws the display list text
	glPopAttrib();  //pops the display list bits
}

//resizing func
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height == 0) height = 1;  //preventing a divide by zero

	glViewport(0, 0, width, height);  //reseting viewpoint

	glMatrixMode(GL_PROJECTION);  //selecting projection matrix
		//projection matrix is responsible to adding perspective to the scene
	glLoadIdentity();  //reseting projection matrix

	//calculating the aspect ratio of the window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);


	glMatrixMode(GL_MODELVIEW);  
}

int InitGL(GLvoid)  //all setup for opengl
{
	glShadeModel(GL_SMOOTH);  //enables smooth shading
		//one benefit ex: smooth shading blends colors bicely across a polygon

	//adusting background color
	//glClearColor(0.99f, 0.37f, 0.33f, 0.0f); //RED,GREEN,BLUE,ALPHA
	glClearColor(0.7f, 0.7f, 0.5f, 0.0f);

	//depth buffer, tracks of how deep objects are into the screen
	glClearDepth(1.0f);  //depth buffer setup
	glEnable(GL_DEPTH_TEST);  //depth testing enabled
	glDepthFunc(GL_LEQUAL);  //type of the depth test

	//telling opengl we want the best perspective correction, MF!
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  //cute perspective calculations

	//building the font
	BuildFont();

	return TRUE;  //init successful
}

int DrawGLScene(GLvoid)  //THIS IS THE DRAWING PART, all drawing will be done in here!
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear The Screen And The Depth Buffer
	glLoadIdentity();                   // Reset The View
	glTranslatef(0.0f, 0.0f, -1.0f);

	glColor3f(1.0f*float(cos(cnt1)), 1.0f*float(sin(cnt2)), 1.0f - 0.5f*float(cos(cnt1 + cnt2)));
	glRasterPos2f(-0.45f + 0.05f*float(cos(cnt1)), 0.35f*float(sin(cnt2)));
	glPrint("HEHEHE MADE IT!");
	cnt1 += 0.051f;
	cnt2 += 0.005f;

	return TRUE;  //success
}

GLvoid KillGLWindow(GLvoid)  //properly kill the window
{
	if (fullscreen)  //is fullscreen enabled?
	{
		ChangeDisplaySettings(NULL, 0);  //so switch back to desktop
		ShowCursor(TRUE);  //mouse cursor
	}

	if (hRC)  //is rendering context running
	{
		if (!wglMakeCurrent(NULL, NULL))
		{
			MessageBox(NULL, "Release of DC and RC failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
			//no parent window, message, title, means a message box with only an OK button, information icon (i)
		}

		if (!wglDeleteContext(hRC))  //is RC ok to delete?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		hRC = NULL;  //set hRC as NULL
	}

	//checking the program has a device context, if so... shut it
	if (hDC && !ReleaseDC(hWnd, hDC))
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;
	}

	//is there a window handle?
	if (hWnd && !DestroyWindow(hWnd))
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;
	}

	if (!UnregisterClass("OpenGL", hInstance))
	{
		MessageBox(NULL, "Could not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;
	}

	//destroy the font
	KillFont();
}

//CREATING OPENGL WINDOW
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint PixelFormat;  //holding the results after searching for a match

	WNDCLASS wc; //all windows belongs to a windows class, MUST

	//style informations, storing the styles so that can be changed
	//styles depending on what kind of window needed to be created
	DWORD dwExStyle;  //extended style
	DWORD dwStyle;  //windows style

	//grabbing the upper left and lower right values of a rectangle
	//with this values, window's area can be adjusted
	RECT WindowRect;
	WindowRect.left = (long)0;
	WindowRect.right = (long)width;  //requested width
	WindowRect.top = (long)0;
	WindowRect.bottom = (long)height;  //requested height

	fullscreen = fullscreenflag;  //set to global fullscreen flag

	hInstance = GetModuleHandle(NULL);  //grab an instance for the window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  //redraw on move, and own DC for window
			 //force the window to redraw whenever it resized or moved, creates a private DC for the window
	wc.lpfnWndProc = (WNDPROC)WndProc;  //WndProc handles messages
	wc.cbClsExtra = 0;  //no extra window data
	wc.cbWndExtra = 0;  //no extra window data
	wc.hInstance = hInstance;  //setting the instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);  //default icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);  //default arrow pointer
	wc.hbrBackground = NULL;  //background already setted up(in GL)
	wc.lpszMenuName = NULL;  //no need for a menu
	wc.lpszClassName = "OpenGL";  //setting the desired class name

	if (!RegisterClass(&wc))  //attempting to register the window class
	{
		MessageBox(NULL, "Rekease of DC and RC failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (fullscreen)  //attempting to fullscreen mode
	{
		DEVMODE dmScreenSettings;  //device mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));  //makes sure memory is cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings); //size of the devmode struct
		dmScreenSettings.dmPelsWidth = width;  //selected screen width
		dmScreenSettings.dmPelsHeight = height; //height
		dmScreenSettings.dmBitsPerPel = bits;  //bits per pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		//CDS_FULSCREEN disappears the start bar
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			//in case of failure, give options: run on windowed mode...
			if (MessageBox(NULL, "Out of Range Error. 'Yes' to open in windowed mode.", "ERROR", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;
			}

			//...or exit
			else
			{
				MessageBox(NULL, "Program is closing.", "EXIT", MB_OK | MB_ICONSTOP);
				return FALSE;
			}
		}
	}

	if (fullscreen)  //still on fullscreen?
	{
		dwExStyle = WS_EX_APPWINDOW;  //window extended style
		dwStyle = WS_POPUP;  //windows style
		ShowCursor(FALSE);  //hiding the mouse
	}

	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;  //window extended style
		dwStyle = WS_OVERLAPPEDWINDOW;  //windows style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);  //adjust window to true requested size
																//in fullscreen, has no effect

	if (!(hWnd = CreateWindowEx(dwExStyle,  //extended style for the window
		"OpenGL",  //class name
		title, //window title
		WS_CLIPSIBLINGS |  //required window style, preventing other windows from drawing on or in the opengl window
		WS_CLIPCHILDREN |  //required window style
		dwStyle,  //selected window style
		0, 0, //window position / (0, 0) is a safe bet
		WindowRect.right - WindowRect.left,  //calculate adjusted window weight
		WindowRect.bottom - WindowRect.top,  //calculate adjusted window height
		NULL,  //no parent window
		NULL,  //no menu
		hInstance,  //instance
		NULL)))  //don't pass anything to WM_CREATE
	{
		KillGLWindow();  //reset display
		MessageBox(NULL, "Window Creation Error", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	static PIXELFORMATDESCRIPTOR pfd =  //pfd tells windows how we want things to be
	{
		sizeof(PIXELFORMATDESCRIPTOR),  //size of pixel format descriptor
		1,  //version number
		PFD_DRAW_TO_WINDOW |  //format must support windows
		PFD_SUPPORT_OPENGL |  //format must support opengl
		PFD_DOUBLEBUFFER,  //must support double buffering
		PFD_TYPE_RGBA,  //request an rgba format
		bits,  //select our color depth
		0, 0, 0, 0, 0, 0,  //color bits ignored
		0,  //no alpha buffer
		0,  //shift bit ignored
		0,  //no accumulation buffer
		0, 0, 0, 0,  //accumulation bits ignored
		(byte)32, // 32Bit Z-Buffer (depth buffer)
		0,  //no stencil buffer
		0,  //no auxiliary buffer
		PFD_MAIN_PLANE,  //main drawing layer
		0,  //reserved
		0, 0, 0  //layer masks ignored
	};

	if (!(hDC = GetDC(hWnd)))  //is device context valid?
	{
		KillGLWindow();
		MessageBox(NULL, "Can't Create a GL device context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))  //did windows find a matching pixel format?
	{
		KillGLWindow();
		MessageBox(NULL, "Pixel format is not valid.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(SetPixelFormat(hDC, PixelFormat, &pfd)))  //did pixel format setted?
	{
		KillGLWindow();
		MessageBox(NULL, "Can't set the PixelFormat", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!(hRC = wglCreateContext(hDC)))  //trying to get a rendering context
	{
		KillGLWindow();
		MessageBox(NULL, "Can't create a GL rendering context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	if (!wglMakeCurrent(hDC, hRC))  //if all things ok, make the rendering context active
	{
		KillGLWindow();
		MessageBox(NULL, "Can't activate the GL rendering context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	//showing, giving priority, giving keyboard focus to window
	ShowWindow(hWnd, SW_SHOW);  //show the window
	SetForegroundWindow(hWnd);  //slightly higher priority
	SetFocus(hWnd);  //keyboard focus
	ReSizeGLScene(width, height);  //set up our perspective GL screen

	if (!InitGL())  //initialize created gl window
	{
		KillGLWindow();
		MessageBox(NULL, "Initialization failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}

	return TRUE;  //successful
}

//this is where all the window messages are dealt with
LRESULT CALLBACK WndProc(HWND hWnd,  //handle for this window
	UINT uMsg,  //msg for this window
	WPARAM wParam,  //additional msg info
	LPARAM lParam)  //additional msg info
{
	switch (uMsg)  //check for windows msgs
	{
	case WM_ACTIVATE:  //watch for window activate msg
	{
		if (!HIWORD(wParam))  //check minimization state
		{
			active = TRUE;  //program is active
		}

		else
		{
			active = FALSE;  //program is no longer active
		}

		return 0;
	}

	case WM_SYSCOMMAND:  //intercept system commands
	{
		switch (wParam)  //check system calls
		{
		case SC_SCREENSAVE:  //screensaver trying to start
		case SC_MONITORPOWER:  //monitor trying to enter powersave
			return 0;  //prevent from happening
		}
		break;  //exit
	}

	case WM_CLOSE:  //a close message recieved
	{
		PostQuitMessage(0);  //send a quit message
		return 0;  //jump back
	}

	case WM_KEYDOWN:  //is a key helding down?
	{
		keys[wParam] = TRUE;  //YES
		return 0;  //jump back
	}

	case WM_KEYUP:  //released key?
	{
		keys[wParam] = FALSE;  //RELEASED!
		return 0;  //jump back
	}

	case WM_SIZE:  //resize opengl window
	{
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  //LoWord: width, HiWord: height
		return 0;
	}
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);  //pass all unhandled msgs to DefWindowProc
}

int WINAPI WinMain(HINSTANCE hInstance,  //instance
	HINSTANCE hPrevInstance,  //previous instance
	LPSTR lpCmdLine,  //command line parameters
	int nCmdShow)  //window show state
{
	MSG msg;  //windows msg struct
	BOOL done = FALSE;

	//ask the user which screen mode they prefer
	if (MessageBox(NULL, "Want to open in fullscreen?", "Fullscreen", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		fullscreen = FALSE;  //opened in windowed mode
	}
	char name_of[] = "SNAKE"; //TRYRYRYRYR
	//creating opengl window
	if (!CreateGLWindow(name_of, game_width, game_height, 32, fullscreen))   //resolutionw
	{
		return 0;  // quit if window was not created
	}

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  //is there a waiting msg?
		{

			if (msg.message == WM_QUIT)  //a quit msg recieved?
			{
				done = TRUE;
			}

			else
			{
				TranslateMessage(&msg);  //translate the msg
				DispatchMessage(&msg);  //dispatch the msg
			}
		}

		else
		{
			if (active)
			{
				if (keys[VK_ESCAPE])  //pressed ESC
				{
					done = TRUE;  //shutdown
				}
				else
				{
					//w.Snakes.clear();  //empty the player count
					while (w.Snakes.size() < player_count)
					{
						w.RegisterSnake();  //first snake registeration
					}

					/* TEXT */
					//setting the area
					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					glLoadIdentity();
					glTranslatef(.0f, .0f, -1.0f);
					//setting color of the text
					glColor3f(.0f, .0f, .0f);
					//position the text on the screen
					//glRasterPos2f(.575f, .35f);
					/*GLfloat score_height = game_height - 0;
					GLfloat score_width = game_width - 0;*/
					glRasterPos2f(.550f, .35f);
					//printing now
					glPrint("Score1: %d", w.Snakes[0].bait_counter);
					glRasterPos2f(.575f, .30f);
					if (w.Snakes.size() > 1) glPrint("Score2: %d", w.Snakes[1].bait_counter);
					/* TEXT END */

					/* GAME */
					w.RenderAllObjects(&buffer);
					//if (w.Snakes[0].GameOver() == true) break;
					//if (s.GameOver()) break;  //KillGLWindow;

					SwapBuffers(hDC);
				}


				/* MOVEMENT OF THE FIRST SNAKE */
				if (keys[VK_LEFT])
				{
					w.Snakes[0].dir = w.Snakes[0].left;
				}

				if (keys[VK_RIGHT])
				{
					w.Snakes[0].dir = w.Snakes[0].right;
				}

				if (keys[VK_UP])
				{
					w.Snakes[0].dir = w.Snakes[0].up;
				}

				if (keys[VK_DOWN])
				{
					w.Snakes[0].dir = w.Snakes[0].down;
				}
				/* END */

				/* MOVEMENT OF THE SECOND SNAKE */
				if (w.Snakes.size() > 1)
				{
					if (keys[VK_NUMPAD4])
					{
						w.Snakes[1].dir = w.Snakes[1].left;
					}

					if (keys[VK_NUMPAD6])
					{
						w.Snakes[1].dir = w.Snakes[1].right;
					}

					if (keys[VK_NUMPAD8])
					{
						w.Snakes[1].dir = w.Snakes[1].up;
					}

					if (keys[VK_NUMPAD5])
					{
						w.Snakes[1].dir = w.Snakes[1].down;
					}
				}
				/* END */

				w.Snakes[0].Move();
				if (w.Snakes.size() > 1) w.Snakes[1].Move();
			}
		}

			if (keys[VK_F1])  //pressed F1
			{
				keys[VK_F1] = FALSE;
				KillGLWindow();
				fullscreen = !fullscreen;
				//recreate opengl window
				if (!CreateGLWindow(name_of, game_width, game_height, 32, fullscreen))  //1920x1080 for fullscreen or 1024 * 728
				{
					return 0;  //returning fullscreen
				}
			}
	}

	//shutdown
	//w.UnregisterObjects();  //deletion of registered game objects from the memory
	KillGLWindow();  //kill the window
	return (msg.wParam);  //exit the program
}