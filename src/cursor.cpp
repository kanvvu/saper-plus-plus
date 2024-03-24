#include "cursor.hpp"

Cursor::Cursor()
{
	leftClick = false;
	rightClick = false;
	cursor.w = 1;
	cursor.h = 1;

	cursor.x = 0;
	cursor.y = 0;
}

Cursor::~Cursor()
{}

bool Cursor::collision(SDL_Rect *rect)
{
	return SDL_HasIntersection(rect, &cursor);

}

void Cursor::setPos(int x, int y)
{
	xpos = x;
	ypos = y;

	cursor.x = xpos;
	cursor.y = ypos;
}

void Cursor::reset()
{
	leftClick = false;
	rightClick = false;
	qClick = false;

}