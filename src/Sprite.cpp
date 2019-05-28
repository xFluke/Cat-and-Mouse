#include "Sprite.h"

Sprite::Sprite() {}
Sprite::Sprite(SDL_Rect s, SDL_Rect d) {
	m_rSrc = s;
	m_rDst = d;
}
Sprite::~Sprite() {}

SDL_Rect Sprite::GetSrc() { return m_rSrc; }
SDL_Rect Sprite::GetDst() { return m_rDst; }
SDL_Rect* Sprite::GetSrcP() { return &m_rSrc; }
SDL_Rect* Sprite::GetDstP() { return &m_rDst; }

void Sprite::SetSrc(char c) {
	if (c == 'B' || c == 'i') // Black Tile
		m_rSrc = {};
	else if (c == 'A') // Blue Tile
		m_rSrc = { 0,0,32,32 };
	else if (c == 'O') // Power Up Tile
		m_rSrc = { 32,0,32,32 };
	else if (c == 'P' || c == 'I') // Pellet Tile
		m_rSrc = { 64,0,32,32 };
	else
	{
		m_rSrc = { 96,0,32,32 };
	}
}

void Sprite::SetSrc(SDL_Rect src) {
	m_rSrc = src;
}

void Sprite::SetDst(SDL_Rect d) { 
	m_rDst = d; 
}