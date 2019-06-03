#pragma once
#include "Sprite.h"
#include <SDL.h>

enum C_State {
	IDLE,
	WAKEUP,
	SCATTER,
	SEEK,
	DEATH
};
enum CatDirection {
	C_UP,
	C_RIGHT,
	C_DOWN,
	C_LEFT
};


class Cat : public Sprite {
private:
	bool m_bIsVulnerable = false, m_bIsDead = false, m_bIsMoving = false;
	int m_IDestinationX, m_IDestinationY;
	char dir;
	int m_iTargetX, m_iTargetY;
	int m_State;
	bool checkBound;
	int DirectionPriority[4];
	void DistanceNorth();
	void DistanceEast();
	void DistanceSouth();
	void DistanceWest();

public:
	int distance;
	Cat(SDL_Rect s, SDL_Rect d);
	bool IsVulnerable();
	bool IsDead();
	bool IsMoving();
	char GetDir();
	void SetDir(char d);
	int GetX();
	int GetY();
	int GetDestinationX();
	int GetDestinationY();
	int GetTargetX();
	int GetTargetY();
	void SetTargetX(int x);
	void SetTargetY(int y);
	void SetDestinationX(int destX);
	void SetDestinationY(int destY);
	void SetMoving(bool b);
	void SetState(int state);
	int GetState();
	void SetPriority(int first, int second, int third, int forth);
	int GetPriority(int index);

	void MoveX(int dir);
	void MoveY(int dir);

	void TargetPlayer();
	void TargetScatter();
	void TargetDeath();
	void Seek();
	void Update();

	void SetVulnerable(bool b);
	void Die();
	int frame;
	int frameX;
	int angle;
	SDL_Point center;
};