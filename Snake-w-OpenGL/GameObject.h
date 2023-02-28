#pragma once

class ScreenBuffer;
class GameObject
{
public:
	const int height = 20;
	const int width = 40;
	int x, y;
	bool bait_eaten = true;
	void Render(ScreenBuffer * buffer);  //main
};