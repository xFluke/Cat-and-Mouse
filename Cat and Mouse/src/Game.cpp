#include <cmath>
#include <iostream>
#include "Game.h"
#include <fstream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"

#define ROWS 19
#define COLS 19

Game::Game() {
	m_fps = (Uint32)round(1 / (long double)FPS * 1000);
}

bool Game::Init(const char* title, int xpos, int ypos, int width, int height, int flags) {
	// Attempt to initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) { // 0 is error code meaning success
		std::cout << "SDL init success!" << std::endl;
		// Initialize window
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != nullptr) { // Window init success
			std::cout << "Window creation successful!" << std::endl;
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);

			if (m_pRenderer != nullptr) { // Renderer init success
				std::cout << "Renderer creation success!" << std::endl;

				if (IMG_Init(IMG_INIT_PNG) != 0) {
					// Create pixel maps.
					SDL_Surface* tileSurface = IMG_Load("../Assets/textures/Tiles.png");
					SDL_Surface* playerSurface = IMG_Load("../Assets/textures/PacMan.png");
					SDL_Surface* ghostsSurface = IMG_Load("../Assets/textures/Cats.png");
					m_pTileTexture = SDL_CreateTextureFromSurface(m_pRenderer, tileSurface);
					m_pPlayerTexture = SDL_CreateTextureFromSurface(m_pRenderer, playerSurface);
					m_pGhostsTexture = SDL_CreateTextureFromSurface(m_pRenderer, ghostsSurface);
					SDL_FreeSurface(tileSurface);
					SDL_FreeSurface(playerSurface);
					SDL_FreeSurface(ghostsSurface);
					std::cout << "Pixel maps creation success!" << std::endl;
				}
				else {
					return 1; // Image init fail
				}
			}
			else {
				std::cout << "Renderer init fail!" << std::endl;
				return false; //Renderer init fail
			}
		}
		else {
			std::cout << "Window init fail!" << std::endl;
			return false; // Window init fail
		}
	}
	else {
		std::cout << "SDL init fail!" << std::endl;
		return false; // SDL init fail
	}

	// Build tilemap
	std::ifstream inFile("../Assets/Level0.txt");
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			char temp;
			inFile >> temp;
			m_level.m_Map[row][col].SetSrc(temp);
			m_level.m_Map[row][col].SetTileVariables(temp);
			m_level.m_Map[row][col].SetDst({ 32 * col, 32 * row, 32, 32 });
		}
	}
	inFile.close();

	m_iKeyStates = SDL_GetKeyboardState(NULL);
	// Spawn Player and Cats
	// Starting coordinate: 9, 11
	m_pPlayer = new Player({ 0, 0, 32, 32 }, { 32 * 9, 32 * 11, 32, 32 });

	m_pCats[0] = new Cat({ 0, 0, 32, 32 }, { 32 * 8, 32 * 9, 32, 32 });
	m_pCats[1] = new Cat({ 32, 0, 32, 32 }, { 32 * 9, 32 * 9, 32, 32 });
	m_pCats[2] = new Cat({ 64, 0, 32, 32 }, { 32 * 10, 32 * 9, 32, 32 });
	m_pCats[3] = new Cat({ 96, 0, 32, 32 }, { 32 * 4, 32 * 2, 32, 32 });
	m_bRunning = true;
	return true;
}

bool Game::KeyDown(SDL_Scancode c) {
	if (m_iKeyStates != nullptr) {
		if (m_iKeyStates[c] == true) {
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}

void Game::Update() {
	PlayerGhostsInteractions();
	PlayerMovements();

	//For debugging purposes
	/*cout << "Is Moving: " <<  m_pPlayer->isMoving() << endl;
	cout << "Destination X: " << m_pPlayer->GetDestinationX() << endl;
	cout << "Rect X: " << m_pPlayer->GetDst().x << endl;
	cout << "Destination Y: " << m_pPlayer->GetDestinationY() << endl;
	cout << "Rect Y: " << m_pPlayer->GetDst().y << endl;*/
}

void Game::PlayerGhostsInteractions() {
	if (m_pPlayer->isPoweredUp()) {
		// Player is only powered up for 10 seconds
		if (SDL_GetTicks() - m_powerUpStartTimer > 10000) {
			m_pPlayer->SetPowered(false);

			// After 10 seconds, return ghosts to their original state
			for (int i = 0; i < 4; i++) {
				m_pCats[i]->SetVulnerable(false);
				if (!m_pCats[i]->isDead()) {
					m_pCats[i]->SetSrc({ i * 32, 0, 32, 32 });
				}
			}
		}
	}

	// Handles player eating a power up
	if (m_level.m_Map[m_pPlayer->GetY()][m_pPlayer->GetX()].isPowerUp() && !m_pPlayer->isMoving()) {
		m_pPlayer->SetPowered(true);
		// Change tile to a normal blank tile with its associated variables
		m_level.m_Map[m_pPlayer->GetY()][m_pPlayer->GetX()].SetSrc('B');
		m_level.m_Map[m_pPlayer->GetY()][m_pPlayer->GetX()].SetTileVariables('B');

		m_powerUpStartTimer = SDL_GetTicks();

		// Make cats vulnerable
		for (int i = 0; i < 4; i++) {
			m_pCats[i]->SetVulnerable(true);
		}
	}

	// Handles player eating a cat
	for (int i = 0; i < 4; i++) {
		// If player collides with cat..
		if (SDL_HasIntersection(m_pPlayer->GetDstP(), m_pCats[i]->GetDstP()) && !m_pPlayer->isMoving()) {
			// Destroy cat if powered up
			if (m_pPlayer->isPoweredUp()) {
				m_pCats[i]->Die();

				// TODO: RESPAWN CAT
			}
			// Else player dies
			else {
				m_pPlayer->Die();
				m_bRunning = false;
			}
		}
	}
}

// Tile based movement with obstacle detection
void Game::PlayerMovements() {
	// Player eating a pellet
	if (m_level.m_Map[m_pPlayer->GetY()][m_pPlayer->GetX()].isPoint() && !m_pPlayer->isMoving()) {
		// Change tile to a normal blank tile with its associated variables
		m_level.m_Map[m_pPlayer->GetY()][m_pPlayer->GetX()].SetSrc('B');
		m_level.m_Map[m_pPlayer->GetY()][m_pPlayer->GetX()].SetTileVariables('B');
	}

	// Player movement input handling - Sets the player to the target destination
	if (!m_pPlayer->isDead() && !m_pPlayer->isMoving()) {
		if (KeyDown(SDL_SCANCODE_W)) {
			if (!m_level.m_Map[m_pPlayer->GetY() - 1][m_pPlayer->GetX()].isObstacle()) {
				m_pPlayer->SetDestinationY(m_pPlayer->GetDst().y - 32);
				m_pPlayer->SetDestinationX(m_pPlayer->GetDst().x);
				m_pPlayer->SetMoving(true);
			}
		}
		else if (KeyDown(SDL_SCANCODE_S)) {
			if (!m_level.m_Map[m_pPlayer->GetY() + 1][m_pPlayer->GetX()].isObstacle()) {
				m_pPlayer->SetDestinationY(m_pPlayer->GetDst().y + 32);
				m_pPlayer->SetDestinationX(m_pPlayer->GetDst().x);
				m_pPlayer->SetMoving(true);
			}
		}
		else if (KeyDown(SDL_SCANCODE_A)) {
			if (!m_level.m_Map[m_pPlayer->GetY()][m_pPlayer->GetX() - 1].isObstacle())
			{
				m_pPlayer->SetDestinationX(m_pPlayer->GetDst().x - 32);
				m_pPlayer->SetDestinationY(m_pPlayer->GetDst().y);
				m_pPlayer->SetMoving(true);
			}
		}
		else if (KeyDown(SDL_SCANCODE_D)) {
			if (!m_level.m_Map[m_pPlayer->GetY()][m_pPlayer->GetX() + 1].isObstacle()) {
				m_pPlayer->SetDestinationX(m_pPlayer->GetDst().x + 32);
				m_pPlayer->SetDestinationY(m_pPlayer->GetDst().y);
				m_pPlayer->SetMoving(true);
			}
		}
	}

    // Moves player to target location
	if (m_pPlayer->isMoving()) {
		if (m_pPlayer->GetDestinationX() > m_pPlayer->GetDst().x) {
			m_pPlayer->MoveX(1);
		}
		else if (m_pPlayer->GetDestinationX() < m_pPlayer->GetDst().x) {
			m_pPlayer->MoveX(-1);
		}
		else if (m_pPlayer->GetDestinationY() > m_pPlayer->GetDst().y) {
			m_pPlayer->MoveY(1);
		}
		else if (m_pPlayer->GetDestinationY() < m_pPlayer->GetDst().y) {
			m_pPlayer->MoveY(-1);
		}
		else if (m_pPlayer->GetDestinationX() == m_pPlayer->GetDst().x && m_pPlayer->GetDestinationY() == m_pPlayer->GetDst().y) {
			m_pPlayer->SetMoving(false);
		}
	}
}

void Game::Render() {
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer);
	// Render map
	for (int row = 0; row < ROWS; row++) {
		for (int col = 0; col < COLS; col++) {
			SDL_RenderCopy(m_pRenderer, m_pTileTexture, m_level.m_Map[row][col].GetSrcP(), m_level.m_Map[row][col].GetDstP());
		}
	}

	// Render ghosts
	for (int i = 0; i < 4; i++) {
		SDL_RenderCopy(m_pRenderer, m_pGhostsTexture, m_pCats[i]->GetSrcP(), m_pCats[i]->GetDstP());
	}

	// Render player
	SDL_RenderCopy(m_pRenderer, m_pPlayerTexture, m_pPlayer->GetSrcP(), m_pPlayer->GetDstP());

	SDL_RenderPresent(m_pRenderer);
}

void Game::HandleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			m_bRunning = false;
			break;
		}
	}
}


// Wake() and Sleep() work together to maintain FPS
void Game::Wake() {
	m_start = SDL_GetTicks();
}

void Game::Sleep() {
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps) {
		SDL_Delay(m_fps - m_delta);
	}
}

void Game::Clean() {
	std::cout << "Cleaning game. Bye!" << std::endl;
	SDL_DestroyTexture(m_pTileTexture);
	SDL_DestroyTexture(m_pPlayerTexture);
	SDL_DestroyWindow(m_pWindow);
	SDL_DestroyRenderer(m_pRenderer);
	IMG_Quit();
	SDL_Quit();
}

