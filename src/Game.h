#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "Player.h"
#include "Cat.h"
#include "Level.h"
#define FPS 60

class Game {
private:
	Game();
	~Game() {};

	// Game Engine Variables
	bool m_bRunning;
	const Uint8* m_iKeyStates;
	Uint32 m_start, m_end, m_delta, m_fps, m_powerUpStartTimer;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;
	
	SDL_Texture* m_pTileTexture;
	SDL_Texture* m_pPickupTexture;
	SDL_Texture* m_pPlayerTexture;
	SDL_Texture* m_pGhostsTexture;

	// Game Object Variables
	Player* m_pPlayer; // Point to Player object
	Cat* m_pCats[4];

	// Game Variables
	int m_currLevel = 0;
	Level m_level;
	Level m_bg;
	
	

public:
	static Game* GetInstance() {
		static Game* instance = new Game();
		return instance;
	}
	Player* GetPlayer();
	Level* GetLevel();

	bool Init(const char* title, int xpos, int ypos, int width, int height, int flags);
	bool Running() { return m_bRunning; }
	void Update();
	void Render();
	void HandleEvents();
	void HandlePlayerAbilities();
	void PlayerCatsInteractions();
	void PlayerMovements();
	void CatMovements();
	void Clean();
	void Wake();
	void Sleep();
	bool KeyDown(SDL_Scancode c);
};