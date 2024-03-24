#ifndef game_hpp
#define game_hpp

#include "SDL.h"
#include <iostream>
#include "SDL_ttf.h"

class Game {
public:
	Game();
	~Game();

	void init(const char* title, int xpos, int ypos,bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();
	void makeBoard();
	void setMessageRect();

	bool running() { return isRunning; }
	enum STATES {play,win,lost};
private:
	//enum STATES {play,win,lost};
	STATES currentState;
	bool isRunning;
	SDL_Window *window;
	SDL_Renderer *renderer;
	int x_pos;
	int y_pos;
};

#endif /* game_hpp */