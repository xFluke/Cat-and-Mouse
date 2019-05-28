#pragma once
#include "Sprite.h"
#include <SDL.h>

class Cat : public Sprite {
private:
	bool m_bIsVulnerable = false, m_bIsDead = false;

public:
	Cat(SDL_Rect s, SDL_Rect d);

	bool isVulnerable();
	bool isDead();

	void SetVulnerable(bool b);
	void Die();
};