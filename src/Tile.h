#pragma once
#include "Sprite.h"
#include <SDL.h>

class Tile : public Sprite {
private:
	bool m_bIsObstacle, m_bIsPoint, m_bIsPowerUp, m_bIsIntersection, m_bIsEnterableWall;

public:
	Tile();
	Tile(SDL_Rect s, SDL_Rect d);
	bool isObstacle();
	bool isPoint();
	bool isPowerUp();
	bool isIntersection();
	bool isEnterableWall();

	void SetTileVariables(char c);
};