#pragma once
#include "Sprite.h"
#include <SDL.h>

class Tile : public Sprite {
private:
	bool m_bIsObstacle, m_bIsPoint, m_bIsPowerUp;

public:
	Tile();
	Tile(SDL_Rect s, SDL_Rect d);
	bool isObstacle();
	bool isPoint();
	bool isPowerUp();

	void SetTileVariables(char c);
};