#pragma once
#include "Sprite.h"
#include <SDL.h>

class Player : public Sprite 
{
private:
	bool m_bIsPoweredUp = false, m_bIsDead = false;
	int m_IMoveSpeed = 2;
	int m_iAngle; // angle of mouse
	int m_iFrame = 0; // current frame
	int m_iSprite = 0; //sprite counter
	int m_iFrameMax = 5; //number of frames to display each sprite
	int m_iSpriteMax = 3; //number of sprites in anumation

public:
	Player(SDL_Rect s, SDL_Rect d);
	SDL_Point center; // pivot point of our mouse
	
	void animate(); // animates mouse
	
	bool isPoweredUp();
	bool isDead();

	int GetX();
	int GetY();

	int GetRightEdgeTile(); //return the tile that the right edge of the sprite is located in
	int GetLeftEdgeTile(); //return the tile that the left edge of the sprite is located in
	int GetTopEdgeTile();//return the tile that the top edge of the sprite is located in
	int GetBottomEdgeTile();//return the tile that the bottom edge of the sprite is located in

	
	void SetPowered(bool b);

	void MoveUp();
	void MoveDown();
	void MoveLeft();
	void MoveRight();
	void SetPlayerSpeed(int speed); //allow us to change player speed

	void SetPlayerAngle( int ang); //angle for rotation of image
	int GetPlayerAngle();
	int GetPlayerFrame();

	void Die();
};
