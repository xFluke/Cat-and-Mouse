#include "Tile.h"

Tile::Tile() {}
Tile::Tile(SDL_Rect s, SDL_Rect d) {
	m_rSrc = s;
	m_rDst = d;
}
bool Tile::isObstacle() { return m_bIsObstacle; }
bool Tile::isPoint() { return m_bIsPoint; }
bool Tile::isPowerUp() { return m_bIsPowerUp; }
bool Tile::isIntersection() { return m_bIsIntersection; }

void Tile::SetTileVariables(char c) {
	if (c == 'B') { // No Tile
		m_bIsObstacle = false;
		m_bIsPoint = false;
		m_bIsPowerUp = false;
		m_bIsIntersection = false;
	}
	else if (c == 'A') { // brick tile
		m_bIsObstacle = true;
		m_bIsPoint = false;
		m_bIsPowerUp = false;
		m_bIsIntersection = false;
	}
	else if (c == 'P') { // Point Tile
		m_bIsObstacle = false;
		m_bIsPoint = true;
		m_bIsPowerUp = false;
		m_bIsIntersection = false;
	}
	else if (c == 'O') { // Power Up Tile
		m_bIsObstacle = false;
		m_bIsPoint = false;
		m_bIsPowerUp = true;
		m_bIsIntersection = false;
	}
	else if (c == 'I') {//Intersection with a point
		m_bIsObstacle = false;
		m_bIsPoint = true;
		m_bIsPowerUp = false;
		m_bIsIntersection = true;
	}
	else if (c == 'i') {//blank intersection
		m_bIsObstacle = false;
		m_bIsPoint = false;
		m_bIsPowerUp = false;
		m_bIsIntersection = true;
	}
	else if(c=='d') // default indoor tile
	{
		m_bIsObstacle = true;
		m_bIsPoint = false;
		m_bIsPowerUp = false;
		m_bIsIntersection = false;
		m_rSrc.x = 96;
		m_rSrc.y = 0;
		m_rSrc.w = m_rSrc.h = 32;
	}
}