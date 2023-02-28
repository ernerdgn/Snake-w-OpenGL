#include "Snake.h"
#include "ScreenBuffer.h"
#include <Windows.h>
#include <conio.h>
#include <ctime>

void Snake::Move()
{
	switch (dir)
	{
	case Snake::stop:
		break;
	case Snake::up:
		headY++;
		break;
	case Snake::down:
		headY--;
		break;
	case Snake::left:
		headX--;
		break;
	case Snake::right:
		headX++;
		break;
	case Snake::kill:
		exit(0);
		break;
	default:
		break;
	}

	//adjusting & balancing speed
	if (dir == down || dir == up) Sleep(50);
	else Sleep(25);

	//if sneake goes out of borders
	if (headX > width - 2) headX = 1;
	else if (headX < 1) headX = width - 1;

	if (headY > height - 2) headY = 1;
	else if (headY < 1) headY = height - 1;
}

//bool Snake::GameOver()
//{
//	if (bait_counter > 4)
//	{
//		for (int a = 4; a < bait_counter; a++)
//		{
//			if (Tail[0].x == Tail[a].x && Tail[0].y == Tail[a].y) return true;
//			else return false;
//		}
//	}
//}


void Snake::Eat(ScreenBuffer * buffer)
{

	if (Tail[0].x == buffer->bx && Tail[0].y == buffer->by)
	{
		buffer->bait_eaten = true;
		bait_counter++;
	}

	//tail will follow the head
	for (int a = bait_counter; a > -1; a--)
	{
		int tempx, tempy;
		tempx = Tail[a - 1].x;
		tempy = Tail[a - 1].y;
		Tail[a].x = tempx;
		Tail[a].y = tempy;
	}
}

void Snake::Render(ScreenBuffer * buffer)
{
	//setting snake's head
	Tail[0].x = headX;
	Tail[0].y = headY;

	for (int a = 0; a <= bait_counter; a++)
	{
		//sending head position info
		if (a == 0) buffer->PutCharacter(HEAD, Tail[a].x, Tail[a].y);
		//sending tail info
		else buffer->PutCharacter(TAIL, Tail[a].x, Tail[a].y);
	}
}