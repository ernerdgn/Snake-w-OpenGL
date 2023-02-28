#include <iostream>
#include <stdlib.h>
#include <ctime>
#include "Bait.h"
#include "ScreenBuffer.h"

void Bait::Render(ScreenBuffer * buffer)
{
	srand(time(NULL));
	if (buffer->bait_eaten == true)
	{
		x = (rand() % baX) + 1;
		y = (rand() % baY) + 1;
		buffer->PutCharacter(BAIT, x, y);
		buffer->bait_eaten = false;
	}
}

void Bait::Eat(ScreenBuffer * buffer)
{
	if (x == buffer->hx && y == buffer->hy)
	{
		bait_eaten = true;
	}
}

