#include <windows.h>
#include <conio.h>
#include "ScreenBuffer.h"
#include "Snake.h"
void ScreenBuffer::getXY(int x, int y)  //GETTING ALL NECESSARY POINTS FROM THIS FUNCTION, or not?
{
	COORD cursorPosition;
	cursorPosition.X = x;
	cursorPosition.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);  //console screen buffer
}

//Puts the character to the inner buffer
void ScreenBuffer::PutCharacter(char c, int x, int y)
{
	MAP[y][x] = c;
}

void ScreenBuffer::Clear()
{
	for (int i = 1; i < width; i++)
	{
		for (int j = 1; j < height; j++)
		{
			while (a < b_counter)
			{
				if (MAP[j][i] == '0' || MAP[j][i] == 'o') continue;
				a++;
			}
			if (i == bx && j == by) continue;
			else
			{
				MAP[j][i] = ' ';
			}
		}
	}
}

/* BORDER OF MAP MATRIX */
// x -> ±11.5f
// y -> ±8.125
/*  EQUATIONS
xx = j * 23 / 40;
yy = i * 16.25 / 20;
*/

void SetLocationE()
{
	glLoadIdentity();
	glTranslatef(-11.5f, -8.125f, .0f);
}

void ScreenBuffer::DrawQuad(float x, float y, float r, float g, float b)  //float z can be added to parameters
{
	SetLocationE();
	glTranslatef(x, y, zet);
	glBegin(GL_QUADS);
	//painting
	glColor3f(r, g, b);
	//drawing  TOP RIGHT -> TOP LEFT
	//         -> BOTTOM LEFT -> BOTTOM RIGHT
	glVertex3f(0.15f, 0.15f, .0f);  //tr
	glVertex3f(-0.15f, 0.15f, .0f);  //tl
	glVertex3f(-0.15f, -0.15f, .0f);  //bl
	glVertex3f(0.15f, -0.15f, .0f);  //br
	glEnd();
}

void ScreenBuffer::RenderToScreen(GLvoid)
{
	/* DRAW */
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			/* BAIT */
			if (MAP[i][j] == 'x')
			{
				bx = j;
				by = i;
				bxx = bx * 23 / 40.0;
				byy = by * 16.25 / 20.0;

				DrawQuad(bxx, byy, .0f, .0f, .0f);
			}

			/* HEAD */
			else if (MAP[i][j] == '0')
			{
				hx = j;
				hy = i;
				hxx = hx * 23 / 40.0;
				hyy = hy * 16.25 / 20.0;

				DrawQuad(hxx, hyy, .0f, 0.3f, 0.1f);
			}

			/* TAIL */
			else if (MAP[i][j] == 'o')
			{
				txx = j * 23 / 40.0;
				tyy = i * 16.25 / 20.0;

				DrawQuad(txx, tyy, .0f, 0.7f, 0.2f);
			}
		}
	}
}
