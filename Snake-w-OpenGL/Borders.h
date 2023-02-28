#pragma once

#include "GameObject.h"
#include <string>
#include <iostream>
class Borders : public GameObject
{
public:
	char BORDER = '*';
	void Render(ScreenBuffer * buffer);
};