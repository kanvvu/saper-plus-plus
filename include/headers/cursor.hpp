#pragma once

#include "game.hpp"

class Cursor
{
public:
	Cursor();
	~Cursor();

	bool collision(SDL_Rect *rect);
	void setPos(int x, int y);
	void setLeftClick(bool click) {Cursor::leftClick = click;}
	void setRightClick(bool click) {Cursor::rightClick = click;}
	bool getLeftClick() {return leftClick;}
	bool getRightClick() {return rightClick;}
	void setqClick(bool click) {Cursor::qClick = click;}
	bool getqClick() {return qClick;}
	int getxpos() {return xpos;}
	int getypos() {return ypos;}
	void reset();
private:
	int xpos;
	int ypos;
	bool leftClick;
	bool rightClick;
	bool qClick;
	SDL_Rect cursor;

};