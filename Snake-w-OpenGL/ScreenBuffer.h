#pragma once
#include <Windows.h>
#include <string>
#include <iostream>

//OpenGL
#include <GL/GL.h>
#include <GL/GLU.h>
#include <glut.h>

class Snake;
class ScreenBuffer
{
public:
	const int width = 40;
	const int height = 20;
	const float zet = -20.0f;
	char MAP[20][40];
	int b_counter = -1;
	int hx, hy;
	int bx, by;
	float bxx, byy;
	float hxx = 11.5f;
	float hyy = 8.125f;
	float txx, tyy;
	int a = 0;
	void getXY(int x, int y);
	bool bait_eaten = true;

	//Puts the character to the inner buffer
	void PutCharacter(char c, int x, int y);

	//Clears the buffer with empty character
	void Clear();

	void DrawQuad(float x, float y, float r, float g, float b);

	//Renders buffer to the screen, In our game it is simply "printf"
	void RenderToScreen(GLvoid);
};