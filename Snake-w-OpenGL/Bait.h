#pragma once
#include <string>
#include <iostream>
#include "GameObject.h"
class Bait : public GameObject  //inherit
{
public:
	char BAIT = 'x';
	int baX = width - 2;
	int baY = height - 2;
	void Render(ScreenBuffer * buffer);  //poly
	void Eat(ScreenBuffer * buffer);
};