#pragma once

#include "GameObject.h"
#include <string>
#include <iostream>

#define MAX_SNAKE_LENGTH 50

class Bait;
class Snake : public GameObject  //inherit
{
public:
	GameObject Tail[MAX_SNAKE_LENGTH];
	int headX = width / 2;
	int headY = height / 2;
	int bait_counter = 0;
	char HEAD = '0';
	char TAIL = 'o';
	enum Directions {
		stop,
		up,
		down,
		left,
		right,
		kill
	};
	Directions dir;
	void Move();
	//bool GameOver();
	void Eat(ScreenBuffer * buffer);
	void Render(ScreenBuffer * buffer);  //poly
};