#include "Game.hpp"


Game *game = nullptr;

int main(int argc, char* argv[]) {
	int a = SDL_GetTicks();
	int b = SDL_GetTicks();
	double delta = 0;

	game = new Game();
	game->init("SAPER++", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, false );
	
	while (game->running()){
		a = SDL_GetTicks();
		delta = a - b;

		if(delta > 1000/60.0){
			b = a;

			game->handleEvents();
			game->update();
			game->render();
		}
	}
	

	game->clean();
	std::cout << "SDL Works" << std::endl;
	return 0;
}