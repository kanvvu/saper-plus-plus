#pragma once

#include "game.hpp"
#include "cursor.hpp"
#include "board.hpp"
#include <vector>

class Menu
{
private:
	bool isOn;
	bool *isGameRunning;
	enum STATES {mainMenu, gameMenu, about, skinMenu};
	STATES menuState;
	std::vector<std::string> skins;
	int currentSkin;

public:
	Menu(SDL_Renderer* ren,bool* isGameRunning, Board *board, SDL_Window *window, Game::STATES *state);
	~Menu();

	void update(Cursor cursor);
	void draw(SDL_Renderer* ren);
	bool getIsOn() {return isOn;}
	void setIsOn(bool isOn) {Menu::isOn = isOn;}
	void turnOffGame() {*isGameRunning = false;}

};