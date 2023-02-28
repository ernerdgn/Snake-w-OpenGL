#include "Borders.h"
#include "ScreenBuffer.h"

void Borders::Render(ScreenBuffer * buffer)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			if (i == 0 || i == width - 1) buffer->PutCharacter(BORDER, i, j);
			if (j == 0 || j == height - 1) buffer->PutCharacter(BORDER, i, j);
		}
	}
}