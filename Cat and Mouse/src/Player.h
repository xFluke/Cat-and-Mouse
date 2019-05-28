#pragma once
#include "Sprite.h"
#include <SDL.h>

class Player : public Sprite {
private:
	bool m_bIsMoving = false, m_bIsPoweredUp = false, m_bIsDead = false;
	int m_IDestinationX, m_IDestinationY;
	int m_IMoveSpeed = 1;

public:
	Player(SDL_Rect s, SDL_Rect d);

	bool isMoving();
	bool isPoweredUp();
	bool isDead();

	int GetX();
	int GetY();
	int GetDestinationX();
	int GetDestinationY();
	void SetDestinationX(int destX);
	void SetDestinationY(int destY);
	void SetMoving(bool b);
	void SetPowered(bool b);

	void MoveX(int dir);
	void MoveY(int dir);
	void Die();
};