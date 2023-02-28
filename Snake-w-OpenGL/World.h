#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>

//OpenGL
#include <GL/GL.h>
#include <GL/GLU.h>
#include <glut.h>

class Snake;
class ScreenBuffer;

class World
{
public:
	std::vector<Snake> Snakes;
	void RegisterSnake();
	//void UnregisterObjects();
	void RenderAllObjects(ScreenBuffer* screenBuffer);
};