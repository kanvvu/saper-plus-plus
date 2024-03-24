#include "game.hpp"
#include "TextureManager.hpp" 
#include "cursor.hpp"
#include "text.hpp"
#include "board.hpp"
#include "button.hpp"
#include "menu.hpp"


//unsigned int lastTime = 0, currentTime;
int sec = 0;

Board *board;
Cursor* cursor;
Menu* menu;
Text* text;
Text* timer;
Text* message;
Button* menuBtn;
Button* resetBtn;
SDL_Rect testRect;
SDL_Rect darkBackground;
SDL_Rect rectBombBackground;
// SDL_Rect rectBtn;
SDL_Rect reactHeart;
SDL_Renderer *newRenderer;
SDL_Texture *bombBackground;
// SDL_Texture *resetButton;
// SDL_Texture *menuButton;
SDL_Texture *heart;
int bombCounter;

int GUIoffset;

std::string stextTest = " ";
Text* textTest;


Game::Game()
{}
Game::~Game()
{}

void Game::init(const char* title, int xpos, int ypos, bool fullscreen)
{
	currentState = play;
	int flags = 0;
	if(fullscreen){
		flags = SDL_WINDOW_FULLSCREEN;
	}

	if(TTF_Init() == -1){
		std::cout<<"Faild ttf"<<std::endl;
		isRunning = false;
	}

	if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout<<"Subsystem init!.." << std::endl;

		window = SDL_CreateWindow(title, xpos,ypos,350, 500, flags);
		if(window)
		{
			std::cout<< "Window created!" << std::endl;
		}

		renderer = SDL_CreateRenderer(window,-1,0);
		if(renderer){
			SDL_SetRenderDrawColor(renderer,195,195,195,255);
			std::cout<< "Window renderer!" << std::endl;
		}

		isRunning = true;
	} else{
		isRunning = false;
	}


	board = new Board(renderer,&currentState);

	//board->loadSkin(renderer, "skin1");

	cursor = new Cursor();
	text = new Text("assets/fonts/comic.ttf", 30, "Testin", {0,0,0,0},renderer);
	textTest = new Text("assets/fonts/comic.ttf", 30, "Testin", {0,0,0,0},renderer);
	timer = new Text("assets/fonts/comic.ttf", 15, "Testin", {0,0,0,0},renderer);

	message = new Text("assets/fonts/arial.ttf", 30, "PRZEGRALES", {255,255,255,255},renderer);
	bombBackground = TextureManager::LoadTexture("assets/bombbackground.png",renderer);
	rectBombBackground.h = 42;
	rectBombBackground.w = 105;
	rectBombBackground.x = 4;
	rectBombBackground.y = 12;
	// resetButton = TextureManager::LoadTexture("assets/resetbutton.png",renderer);
	// menuButton = TextureManager::LoadTexture("assets/menubutton.png",renderer);
	// rectBtn.w = 56;
	// rectBtn.h = 26;
	// rectBtn.y = 19;
	menuBtn = new Button("assets/btns/menubutton.png","assets/btns/menuhoverbutton.png", renderer, 190,19,56,26);
	resetBtn = new Button("assets/btns/resetbutton.png","assets/btns/resethoverbutton.png", renderer, 190+66,19,56,26);

	heart = TextureManager::LoadTexture("assets/heart.png",renderer);
	reactHeart.w = 20;
	reactHeart.h = 20;
	reactHeart.x = 115;
	reactHeart.y = 0;
	newRenderer = SDL_CreateRenderer(window,-1,0);
	SDL_SetRenderDrawColor(newRenderer,100,100,100,255);
	SDL_RenderFillRect(newRenderer, &testRect);
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

	menu = new Menu(renderer,&isRunning, board, window, &currentState);

	SDL_Rect newtestrect = message->getTextRect();
	int xwinpos, ywinpos;
	SDL_GetWindowSize(window,&xwinpos,&ywinpos);
	testRect.w = newtestrect.w+30;
	testRect.h = newtestrect.h+10;
	testRect.x = xwinpos/2 - testRect.w/2;
	testRect.y = ywinpos/2 - testRect.h/2;

	darkBackground.x = darkBackground.y = 0;
	darkBackground.y = 64;
	darkBackground.w = xwinpos;
	darkBackground.h = ywinpos;
	//bombCounter = mines;
}

void Game::setMessageRect()
{
	SDL_Rect newtestrect = message->getTextRect();
	int xwinpos, ywinpos;
	SDL_GetWindowSize(window,&xwinpos,&ywinpos);
	testRect.w = newtestrect.w+30;
	testRect.h = newtestrect.h+10;
	testRect.x = xwinpos/2 - testRect.w/2;
	testRect.y = ywinpos/2 - testRect.h/2;

	darkBackground.x = darkBackground.y = 0;
	darkBackground.y = 64;
	darkBackground.w = xwinpos;
	darkBackground.h = ywinpos;

}

void Game::handleEvents()
{
	
	bool isMovin = false;
	SDL_Event event;
	SDL_PollEvent(&event);
	switch(event.type) {
		case SDL_QUIT:
			isRunning = false;
			break;

		case SDL_KEYDOWN:
			switch(event.key.keysym.sym){
				case SDLK_ESCAPE:
					isRunning = false;
					break;
				case SDLK_r:
					std::cout<<"RESET"<<std::endl;
					board->reset();
					currentState = play;
					break;
				case SDLK_q:
					cursor->setqClick(true);
					break;
			}
			break;
			
		case SDL_MOUSEMOTION:
				isMovin = true;
				break;
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT)
				cursor->setLeftClick(true);
			if (event.button.button == SDL_BUTTON_RIGHT)
				cursor->setRightClick(true);
			break;

		default:
			break;
	}

	if(isMovin){
		x_pos = event.motion.x;
		y_pos = event.motion.y;

		cursor->setPos(x_pos,y_pos);
	}
	//system("cls");
	//std::cout<<stextTest;
}


void Game::update()
{
	if(menu->getIsOn())
	{
		menu->update(*cursor);
	} 
	else
	{
		menuBtn->update(*cursor);
		resetBtn->update(*cursor);

		if(resetBtn->getCollision() && cursor->getLeftClick())
		{
			board->reset();
			currentState = play;
		}

		if(menuBtn->getCollision() && cursor->getLeftClick())
		{
			menu->setIsOn(true);
			SDL_SetWindowSize(window,350,500);
		}

		switch(currentState){
			case play:
			{
				board->Update(*cursor);
				bombCounter = board->getMinesNumber();
				text->setMessage((std::to_string(bombCounter - board->flagged)).c_str(), {255,255,255,255});

				std::string stime = "";
				unsigned int etime = board->getElapsedTime();

				if( (((etime)/(1000*60) )%60) < 10 ) stime.append("0");
				stime.append(std::to_string(((etime)/ (1000*60))%60));
				stime.append(":");
				if( (((etime)/1000)%60) < 10 ) stime.append("0");
				stime.append(std::to_string(((etime)/1000)%60));
				

				timer->setMessage((stime).c_str(), {255,255,255,255});


				break;
			}
			case lost:
			{
				//board->Update(*cursor,&currentState);
				message->setMessage("YOU LOST", {255,255,255,255});
				break;
			}
			case win:
			{
				message->setMessage("YOU WON", {255,255,255,255});
				break;
			}
		}
	}

	cursor->reset();

}

void Game::render()
{
	if(menu->getIsOn())
	{
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);
		menu->draw(renderer);
		textTest->display(0,0,renderer);
		SDL_RenderPresent(renderer);

	}
	else
	{
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);

		board->DrawBoard(renderer,*cursor);
		switch(currentState){
			case play:
			{
				break;
			}
			case lost:
			{
				setMessageRect();
				SDL_SetRenderDrawColor(renderer,0,0,0,50);
				SDL_RenderFillRect(renderer, &darkBackground);
				SDL_SetRenderDrawColor(renderer,0,0,0,150);
				SDL_RenderFillRect(renderer, &testRect);
				message->display(testRect.x+15,testRect.y+5,renderer);
				break;
			}
			case win:
			{
				setMessageRect();
				SDL_SetRenderDrawColor(renderer,0,0,0,50);
				SDL_RenderFillRect(renderer, &darkBackground);
				SDL_SetRenderDrawColor(renderer,0,0,0,150);
				SDL_RenderFillRect(renderer, &testRect);
				message->display(testRect.x+15,testRect.y+5,renderer);
				break;
				break;
			}
		}
		for(int i = 0;i<board->getLives();i++){
			//reactHeart.x = 115;
			reactHeart.y = 20*i;
			SDL_RenderCopy(renderer,heart,NULL,&reactHeart);
		}

		//rectBtn.x = 190;
		//SDL_RenderCopy(renderer,menuButton,NULL,&rectBtn);
		//rectBtn.x = 190 + rectBtn.w + 10;
		//SDL_RenderCopy(renderer,resetButton,NULL,&rectBtn);
		menuBtn->draw(renderer);
		resetBtn->draw(renderer);

		SDL_RenderCopy(renderer,bombBackground,NULL,&rectBombBackground);
		text->display(50,10,renderer);
		timer->display(142,20,renderer);
		SDL_RenderPresent(renderer);
	}

}

void Game::clean()
{
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyRenderer(newRenderer);
	TTF_Quit();
	SDL_Quit();
	std::cout<<"Game cleanded" <<std::endl;
}