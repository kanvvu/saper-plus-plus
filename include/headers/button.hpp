#pragma once

#include "game.hpp"
#include "cursor.hpp"

class Button
{
private:
	SDL_Rect rectButton;
	SDL_Texture *textureButton;
	SDL_Texture *textureHoverButton;
	bool clicked;
	bool collision;
public:
	Button(const char* texture,const char* textureHover,SDL_Renderer* ren,int x,int y,int w,int h);
	~Button();


	void update(Cursor cursor);
	bool getCollision() {return collision;}
	void setPos(int x,int y) {rectButton.x = x; rectButton.y = y;}
	void draw(SDL_Renderer* ren);

};