#include "menu.hpp"
#include "button.hpp"
#include "TextureManager.hpp"
#include "board.hpp"
#include "text.hpp"
#include "FileUtils.hpp"


int gameW;
int gameH;
int gameM;
int gameL;
Board * gameBoard;
SDL_Window * gameWindow;
Game::STATES *gameState;
SDL_Renderer *gameRenderer;

Text* textWidth;
Text* textHeight;
Text* textLives;
Text* textBombs;

Text* textSkin;

Button *playBtn;
Button *exitBtn;

Button *skinBtn;



Button *easyModeBtn;
Button *normalModeBtn;
Button *hardModeBtn;
Button *backBtn;

Button *plusBtn;
Button *minusBtn;

SDL_Texture* gameMenuBackground;


Menu::Menu(SDL_Renderer* ren,bool* isGameRunning, Board* board,SDL_Window *window, Game::STATES *state)
{
	gameBoard = board;
	gameWindow = window;
	gameState = state;
	gameRenderer = ren;
	playBtn = new Button("assets/btns/playbutton.png","assets/btns/playhoverbutton.png",ren, 98,140,155,67);
	exitBtn = new Button("assets/btns/exitbutton.png","assets/btns/exithoverbutton.png",ren, 98,220,155,67);

	skinBtn = new Button("assets/btns/skinbutton.png","assets/btns/skinhoverbutton.png",ren, 98,300,40,40);

	easyModeBtn = new Button("assets/btns/easybutton.png","assets/btns/easyhoverbutton.png",ren, 105,9,141,56);
	normalModeBtn = new Button("assets/btns/normalbutton.png","assets/btns/normalhoverbutton.png",ren, 105,78,141,56);
	hardModeBtn = new Button("assets/btns/hardbutton.png","assets/btns/hardhoverbutton.png",ren, 105,152,141,56);
	backBtn = new Button("assets/btns/backbutton.png","assets/btns/backhoverbutton.png",ren, 10,451,30,30);

	plusBtn = new Button("assets/btns/plusbutton.png","assets/btns/plushoverbutton.png",ren, 78,371,40,40);
	minusBtn = new Button("assets/btns/minusbutton.png","assets/btns/minushoverbutton.png",ren, 233,371,40,40);

	textWidth = new Text("assets/fonts/arial.ttf",30,"0", {255,255,255,255},ren);
	textHeight = new Text("assets/fonts/arial.ttf",30,"0", {255,255,255,255},ren);
	textLives = new Text("assets/fonts/arial.ttf",30,"0", {255,255,255,255},ren);
	textBombs = new Text("assets/fonts/arial.ttf",30,"0", {255,255,255,255},ren);

	textSkin = new Text("assets/fonts/arial.ttf",24,"Default", {255,255,255,255},ren);

	gameMenuBackground = TextureManager::LoadTexture("assets/menubackground.png",ren);
	menuState = mainMenu;
	isOn = true;
	Menu::isGameRunning = isGameRunning;
	gameW = 10;
	gameH = 10;
	gameM = 10;
	gameL = 1;

	currentSkin = 0;
	skins = FileUtils::getDirs("skins/");
	skins.erase(skins.begin());
}

void Menu::update(Cursor cursor)
{
	switch(menuState)
	{
		case mainMenu:
		{
			playBtn->update(cursor);
			exitBtn->update(cursor);
			skinBtn->update(cursor);
			easyModeBtn->update(cursor);
			normalModeBtn->update(cursor);
			hardModeBtn->update(cursor);

			if(cursor.getLeftClick() && playBtn->getCollision())
			{
				menuState = gameMenu;
			}
			if(cursor.getLeftClick() && exitBtn->getCollision())
			{
				turnOffGame();
			}
			if(cursor.getLeftClick() && skinBtn->getCollision())
			{
				if(++currentSkin == skins.size()){
					currentSkin = 0;
					textSkin->setMessage("Default", {255,255,255,255});
					gameBoard->defaultSkin(gameRenderer);
				}
				else{
					textSkin->setMessage((skins[currentSkin]).c_str(), {255,255,255,255});
					gameBoard->loadSkin(gameRenderer, skins[currentSkin]);
				}
					
			}
			break;
		}
		case gameMenu:
		{
			playBtn->update(cursor);
			easyModeBtn->update(cursor);
			normalModeBtn->update(cursor);
			hardModeBtn->update(cursor);
			plusBtn->update(cursor);
			minusBtn->update(cursor);
			backBtn->update(cursor);

			textWidth->setMessage((std::to_string(gameW)).c_str(), {255,255,255,255});
			textHeight->setMessage((std::to_string(gameH)).c_str(), {255,255,255,255});
			textBombs->setMessage((std::to_string(gameM)).c_str(), {255,255,255,255});
			textLives->setMessage((std::to_string(gameL)).c_str(), {255,255,255,255});


			if(cursor.getLeftClick() && easyModeBtn->getCollision())
			{
				gameW = 10;
				gameH = 10;
				gameM = 10;
			}

			if(cursor.getLeftClick() && normalModeBtn->getCollision())
			{
				gameW = 15;
				gameH = 15;
				gameM = 36;
			}


			if(cursor.getLeftClick() && hardModeBtn->getCollision())
			{
				gameW = 20;
				gameH = 20;
				gameM = 68;
			}

			if(cursor.getLeftClick() && plusBtn->getCollision())
			{
				if(gameL<3) gameL++;
			}

			if(cursor.getLeftClick() && minusBtn->getCollision())
			{
				if(gameL>1) gameL--;
			}

			if(cursor.getLeftClick() && backBtn->getCollision())
			{
				menuState = mainMenu;
			}

			if(cursor.getqClick())
			{
				std::cout<<"Podaj wysokosc, szerkosc, liczbe min: ";
				std::cin>>gameW>>gameH>>gameM;
			}

			if(cursor.getLeftClick() && playBtn->getCollision())
			{
				isOn=false;
				gameBoard->init(gameW,gameH,gameM,gameL);
				gameBoard->reset();
				*gameState = Game::play;
				SDL_SetWindowSize(gameWindow,gameW*32,(gameH+2)*32);
				menuState = gameMenu;
			}

			break;
		}
		case about:
		{
			break;
		}
	}

}

void Menu::draw(SDL_Renderer* ren)
{
	switch(menuState)
	{
		case mainMenu:
		{
			playBtn->setPos(98,140);
			playBtn->draw(ren);
			exitBtn->draw(ren);
			skinBtn->setPos(0,460);
			skinBtn->draw(ren);
			textSkin->display(45,465,ren);
			break;
		}
		case gameMenu:
		{
			SDL_RenderCopy(ren,gameMenuBackground,NULL,NULL);
			easyModeBtn->draw(ren);
			normalModeBtn->draw(ren);
			hardModeBtn->draw(ren);
			plusBtn->draw(ren);
			minusBtn->draw(ren);
			playBtn->setPos(98,424);
			playBtn->draw(ren);
			backBtn->draw(ren);
			textWidth->display(142,218,ren);
			textHeight->display(142,262,ren);
			textBombs->display(142,306,ren);
			textLives->display(164,371,ren);
			break;
		}
		case about:
		{
			break;
		}
	}

}