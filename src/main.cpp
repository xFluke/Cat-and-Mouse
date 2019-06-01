#include <SDL.h>
#include "Game.h"
#include <iostream>
#include <Windows.h>

int main(int argc, char* argv[]) {
	AllocConsole();
	freopen("CON", "w", stdout);

	Game* game = Game::GetInstance();
	//increase of X,Y by 320 for border
	if (game->Init("Cat and Mouse", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 992, 992, 0) == false) {
		return 1;
	}
	while (game->Running()) {
		game->Wake();
		game->HandleEvents();
		game->Update();
		game->Render();
		if (game->Running()) {
			game->Sleep();
		}
	}
	game->Clean();
	return 0;
}
