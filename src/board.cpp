#include "board.hpp"
#include "TextureManager.hpp"
#include "cursor.hpp"
#include "FileUtils.hpp"
#include <ctime>

bool firstMoveWasMade = false;
unsigned int lastTime = 0, currentTime = 0, elapsedTime = 0;



Board::Board(SDL_Renderer* ren, Game::STATES* state)
{
	boardState = state;
	defaultSkin(ren);

	rows = 5;
	columns = 5;

	src.x = src.y = 0;
	src.w = src.h = 32;
	dest.w = dest.h = 32;

	Board::xoffset = 0;
	Board::yoffset = 32 * 2;

	Board::flagged = 0;
	Board::lives = 3;
	tempLives = 3;

	srand(time(NULL));

	mines_number = 5;

}

void Board::defaultSkin(SDL_Renderer* ren) {
	empty = TextureManager::LoadTexture("assets/empty.png", ren);
	bomb = TextureManager::LoadTexture("assets/bomb.png", ren);
	covered = TextureManager::LoadTexture("assets/covered.png", ren);
	flag = TextureManager::LoadTexture("assets/flag.png", ren);
	field1[0] = TextureManager::LoadTexture("assets/1.png", ren);
	field1[1] = TextureManager::LoadTexture("assets/2.png", ren);
	field1[2] = TextureManager::LoadTexture("assets/3.png", ren);
	field1[3] = TextureManager::LoadTexture("assets/4.png", ren);
	field1[4] = TextureManager::LoadTexture("assets/5.png", ren);
	field1[5] = TextureManager::LoadTexture("assets/6.png", ren);
	field1[6] = TextureManager::LoadTexture("assets/7.png", ren);
	field1[7] = TextureManager::LoadTexture("assets/8.png", ren);
}

void Board::loadSkin(SDL_Renderer* ren, const std::string& skin) {
	// Sciezki do katalogow z teksturkami
	const std::string skinPath = "skins/" + skin + "/";
	const std::string assetsPath = "assets/";

	// Pobranie sciezek do pngsow z katalogow
	std::vector<std::string> skinPngs = FileUtils::getAllFiles(skinPath, ".png");
	std::vector<std::string> assetsPngs = FileUtils::getAllFiles(assetsPath, ".png");

	std::map<std::string, std::string> tempMap;

	for(int i=0; i< assetsPngs.size(); i++){
		std::string pngName = FileUtils::getFileName(assetsPngs[i]);
		std::string pngPath = assetsPngs[i];
		tempMap[pngName] = pngPath;
	}

	for(int i=0; i< skinPngs.size(); i++){
		std::string pngName = FileUtils::getFileName(skinPngs[i]);
		std::string pngPath = skinPngs[i];
		tempMap[pngName] = pngPath;
	}

	// for (int i = 0; i < assetsPngs.size(); i++) {
	// 	std::string pngName = FileUtils::getFileName(assetsPngs[i]);
	// 	std::string pngPath = tempMap[pngName];
	// 	textureMap[pngName] = TextureManager::LoadTexture(pngPath.c_str(), ren);
	// }

	for (const auto& value : tempMap){
		std::string pngName = value.first;
		std::string pngPath = value.second;
		textureMap[pngName] = TextureManager::LoadTexture(pngPath.c_str(), ren);
	}

	empty = textureMap["empty"];
	bomb = textureMap["bomb"];
	covered = textureMap["covered"];
	flag = textureMap["flag"];

	for(int i = 0; i<8; i++)
		field1[i] = textureMap[std::to_string(i+1)];
}

void Board::init(int width, int height, int mines, int lives)
{
	rows = height;
	columns = width;

	board = new Field * [rows];
	for (int i = 0; i < rows; i++)
	{
		board[i] = new Field[columns];
	}
	LoadBoard();

	Board::lives = lives;
	tempLives = lives;
	mines_number = mines;
	fill_mines(mines_number);
}

void Board::LoadBoard()
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			board[i][j].setValue(0);
			board[i][j].setClicked(false);
			board[i][j].setFlag(false);
		}
	}
}

void Board::Update(Cursor cursor)
{
	SDL_Rect temp;
	temp.w = 32;
	temp.h = 32;

	if (firstMoveWasMade) {
		currentTime = SDL_GetTicks();
		elapsedTime = currentTime - lastTime;

	}
	int covered = 0;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			temp.x = j * 32 + Board::xoffset;
			temp.y = i * 32 + Board::yoffset;

			if (cursor.getLeftClick() && cursor.collision(&temp) && !board[i][j].isFlag() && board[i][j].isClicked())
			{
				uncoverAround(j, i);

			}

			if (cursor.getLeftClick() && cursor.collision(&temp) && !board[i][j].isFlag() && !board[i][j].isClicked())
			{
				if (!firstMoveWasMade)
				{
					firstMoveWasMade = true;
					lastTime = SDL_GetTicks();
				}
				if (board[i][j].getValue() == 9) {
					changeState(Game::lost);
					return;
				}
				isValid(j, i);
				uncover(j, i);

			}

			if (cursor.getRightClick() && cursor.collision(&temp))
			{
				if (board[i][j].isClicked() == false && board[i][j].isFlag() == false && flagged < mines_number) {
					board[i][j].setFlag(true);
					flagged++;
				}
				else if (board[i][j].isClicked() == false && board[i][j].isFlag() == true) {
					board[i][j].setFlag(false);
					flagged--;
				}

			}


		}
	}


	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++) {
			if (!board[i][j].isClicked()) covered++;

		}
	}
	if (mines_number == covered) {
		changeState(Game::win);
		return;
	}

}

unsigned int Board::getElapsedTime()
{
	return elapsedTime;
}
void Board::reset() {
	LoadBoard();
	fill_mines(mines_number);
	flagged = 0;
	lives = tempLives;
	firstMoveWasMade = false;
	elapsedTime = 0;
}


void Board::showMines() {
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++) {
			if (board[i][j].getValue() == 9) board[i][j].setClicked(true);
		}
	}
}

void Board::changeState(Game::STATES state) {
	Board::lives--;
	if (Board::lives == 0) {
		showMines();
		*boardState = state;
	}
}


void Board::fill_mines(int number_of_mines) {
	int x;
	int y;

	while (number_of_mines > 0) {
		x = rand() % rows;
		y = rand() % columns;

		if (board[x][y].getValue() != 9) {
			board[x][y].setValue(9);

			for (int k = -1; k < 2; k++) {
				for (int h = -1; h < 2; h++) {
					if ((x + h) < 0 || (y + k) < 0) continue;
					if ((x + h) > rows - 1 || (y + k) > columns - 1) continue;

					if (board[x + h][y + k].getValue() == 9) continue;
					board[x + h][y + k].setValue(board[x + h][y + k].getValue() + 1);
				}
			}
			number_of_mines--;
		}
	}
}

void Board::uncover(int x, int y) {

	if (!isValid(x, y)) return;
	if (board[y][x].isClicked() == true) return;

	if (board[y][x].getValue() == 9) {
		std::cout << "PRZEGRALES!" << std::endl;
		changeState(Game::lost);
	}
	if (board[y][x].isFlag()) return;

	if (board[y][x].getValue() != 9 && board[y][x].isClicked() == false) {
		board[y][x].setClicked(true);
	}

	if (board[y][x].getValue() != 0) return;

	uncover(x - 1, y - 1);
	uncover(x, y - 1);
	uncover(x + 1, y - 1);
	uncover(x - 1, y);
	uncover(x, y);
	uncover(x + 1, y);
	uncover(x - 1, y + 1);
	uncover(x, y + 1);
	uncover(x + 1, y + 1);
}

bool Board::isValid(int x, int y) {
	if (x >= 0 && x < columns) {
		if (y >= 0 && y < rows)
			return true;
	}

	return false;
}

int Board::countFlagged(int x, int y) {
	int flagged = 0;
	if (isValid(x - 1, y - 1)) {
		if (board[y - 1][x - 1].isFlag()) flagged++;
	}
	if (isValid(x, y - 1)) {
		if (board[y - 1][x].isFlag()) flagged++;
	}
	if (isValid(x + 1, y - 1)) {
		if (board[y - 1][x + 1].isFlag()) flagged++;
	}
	if (isValid(x - 1, y)) {
		if (board[y][x - 1].isFlag()) flagged++;
	}
	if (isValid(x + 1, y)) {
		if (board[y][x + 1].isFlag()) flagged++;
	}
	if (isValid(x - 1, y + 1)) {
		if (board[y + 1][x - 1].isFlag()) flagged++;
	}
	if (isValid(x, y + 1)) {
		if (board[y + 1][x].isFlag()) flagged++;
	}
	if (isValid(x + 1, y + 1)) {
		if (board[y + 1][x + 1].isFlag()) flagged++;
	}

	return flagged;

}

void Board::uncoverAround(int x, int y) {
	if (countFlagged(x, y) >= board[y][x].getValue()) {

		if (isValid(x - 1, y) && !board[y][x - 1].isFlag()) uncover(x - 1, y);
		if (isValid(x - 1, y - 1) && !board[y - 1][x - 1].isFlag()) uncover(x - 1, y - 1);
		if (isValid(x - 1, y + 1) && !board[y + 1][x - 1].isFlag()) uncover(x - 1, y + 1);
		if (isValid(x, y) && !board[y][x].isFlag()) uncover(x, y);
		if (isValid(x, y - 1) && !board[y - 1][x].isFlag()) uncover(x, y - 1);
		if (isValid(x, y + 1) && !board[y + 1][x].isFlag()) uncover(x, y + 1);
		if (isValid(x + 1, y) && !board[y][x + 1].isFlag()) uncover(x + 1, y);
		if (isValid(x + 1, y - 1) && !board[y - 1][x + 1].isFlag()) uncover(x + 1, y - 1);
		if (isValid(x + 1, y + 1) && !board[y + 1][x + 1].isFlag()) uncover(x + 1, y + 1);
	}

}

void Board::DrawBoard(SDL_Renderer* ren, Cursor cursor)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			dest.x = j * 32 + Board::xoffset;
			dest.y = i * 32 + Board::yoffset;

			if (board[i][j].isFlag()) TextureManager::Draw(flag, src, dest, ren);
			else if (!board[i][j].isClicked()) TextureManager::Draw(covered, src, dest, ren);
			else {
				switch (board[i][j].getValue())
				{
				case 0:
					TextureManager::Draw(empty, src, dest, ren);
					break;
				case 1:
					TextureManager::Draw(field1[0], src, dest, ren);
					break;
				case 2:
					TextureManager::Draw(field1[1], src, dest, ren);
					break;
				case 3:
					TextureManager::Draw(field1[2], src, dest, ren);
					break;
				case 4:
					TextureManager::Draw(field1[3], src, dest, ren);
					break;
				case 5:
					TextureManager::Draw(field1[4], src, dest, ren);
					break;
				case 6:
					TextureManager::Draw(field1[5], src, dest, ren);
					break;
				case 7:
					TextureManager::Draw(field1[6], src, dest, ren);
					break;
				case 8:
					TextureManager::Draw(field1[7], src, dest, ren);
					break;
				case 9:
					TextureManager::Draw(bomb, src, dest, ren);
					break;

				}
			}
		}
	}
}