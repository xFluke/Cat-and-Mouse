#include "Cat.h"

Cat::Cat(SDL_Rect s, SDL_Rect d) {
	m_rSrc = s;
	m_rDst = d;
}

bool Cat::isVulnerable() { return m_bIsVulnerable; }
bool Cat::isDead() { return m_bIsDead; }

void Cat::SetVulnerable(bool b) {
	if (b) {
		m_bIsVulnerable = true;
		if (!m_bIsDead) {
			m_rSrc = { 128, 0, 32, 32 };
		}
	}
	else {
		m_bIsVulnerable = false;
	}
}

void Cat::Die() {
	m_bIsDead = true;
	m_rSrc = {};
}