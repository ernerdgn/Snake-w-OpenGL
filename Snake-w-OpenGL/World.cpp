#include "World.h"
#include "ScreenBuffer.h"

#include "Bait.h"
#include "Snake.h"
Bait b; 
std::vector<Snake> Snakes;
//Bait* bPtr = new Bait();

void World::RegisterSnake()
{
	Snake* sPtr = new Snake();
	Snakes.push_back(*sPtr);
	delete sPtr;
}

//void World::UnregisterObjects()
//{
	//delete sPtr;
	//delete bPtr;
//}

void World::RenderAllObjects(ScreenBuffer * screenBuffer)
{

	screenBuffer->Clear();
	b.Render(screenBuffer);
	//bPtr->Render(screenBuffer);
	Snakes[0].Render(screenBuffer);
	if(Snakes.size() > 1) Snakes[1].Render(screenBuffer);
	screenBuffer->RenderToScreen();

	Snakes[0].Eat(screenBuffer);
	if (Snakes.size() > 1) Snakes[1].Eat(screenBuffer);
	b.Eat(screenBuffer);
	//bPtr->Eat(screenBuffer);

	return;
}