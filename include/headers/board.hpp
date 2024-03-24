#pragma once
#include "game.hpp"
#include "field.hpp"
#include "cursor.hpp"
#include <map>

class Board
{
public:
	Board(SDL_Renderer* ren, Game::STATES* state);
	~Board();

	void LoadBoard();
	void Update(Cursor cursor);
	void DrawBoard(SDL_Renderer* ren, Cursor cursor);
	void fill_mines(int number_of_mines);
	void uncover(int x, int y);
	void showMines();
	int countFlagged(int x, int y);
	void uncoverAround(int x, int y);
	bool isValid(int x, int y);
	int getMinesNumber() { return mines_number; }
	void reset();
	void changeState(Game::STATES state);
	int getLives() { return Board::lives; }
	void init(int width, int height, int mines, int lives);
	unsigned int getElapsedTime();

	void defaultSkin(SDL_Renderer* ren);
	void loadSkin(SDL_Renderer* ren, const std::string& path);

	int flagged;
private:

	SDL_Rect src, dest;

	SDL_Texture* empty;
	SDL_Texture* bomb;
	SDL_Texture* covered;
	SDL_Texture* flag;
	SDL_Texture* field1[8];

	std::map<std::string, SDL_Texture*> textureMap;

	Game::STATES* boardState;
	int lives;
	int tempLives;
	int rows;
	int columns;
	int flags_counter;
	int fieldSize;
	int xoffset;
	int yoffset;
	int mines_number;
	Field** board;
};