#include "Cat.h"
#include "Game.h"
#include <iostream>
#include <vector>

Cat::Cat(SDL_Rect s, SDL_Rect d) {
	m_rSrc = s;
	m_rDst = d;
	dir = 'd';
	m_moveSpeed = 2;
	angle = 0;
	center.x = center.y = m_rDst.w / 2;
	m_State = C_State::SEEK;
	checkBound = false;
}


bool Cat::IsVulnerable() { return m_bIsVulnerable; }
bool Cat::IsDead() { return m_bIsDead; }

char Cat::GetDir() { return dir; }

void Cat::SetDir(char d) { dir = d; }

int Cat::GetX() { return (m_rDst.x + m_rDst.w / 2) / m_rDst.w; }

int Cat::GetY() { return (m_rDst.y + m_rDst.h / 2) / m_rDst.h; }

int Cat::GetDestinationX() { return m_IDestinationX; }

int Cat::GetDestinationY() { return m_IDestinationY; }

int Cat::GetTargetX() { return m_iTargetX; }

int Cat::GetTargetY() { return m_iTargetY; }

void Cat::SetTargetX(int x) { m_iTargetX = x; }

void Cat::SetTargetY(int y) { m_iTargetY = y; }

void Cat::SetDestinationX(int destX) { m_IDestinationX = destX; }

void Cat::SetDestinationY(int destY) { m_IDestinationY = destY; }

void Cat::SetMoving(bool b) { m_bIsMoving = b; }

bool Cat::IsMoving() { return m_bIsMoving; }

void Cat::Animate()
{
	if (m_iFrame == m_iFrameMax)
	{
		m_iFrame = 0;
		m_iSprite++;
		if (m_iSprite == m_iSpriteMax)
		{
			m_iSprite = 0;
			m_rSrc.x -= m_rSrc.w * 3;
		}
		m_rSrc.x += m_rSrc.w;
	}
	m_iFrame++;
}

void Cat::ResetCell() { m_iSprite = 0; m_iFrame = 0; }

void Cat::SetState(int state) { m_State = state; }
int Cat::GetState() { return m_State; }
void Cat::SetPriority(int first, int second, int third, int forth)
{
	DirectionPriority[0] = first;
	DirectionPriority[1] = second;
	DirectionPriority[2] = third;
	DirectionPriority[3] = forth;
}
int Cat::GetPriority(int index) { return DirectionPriority[index]; }

void Cat::MoveX(int dir) 
{ 
	m_rDst.x += dir * m_moveSpeed;
	if (dir>0)
	{
		angle = 270;
	}
	else if (dir<0)
	{
		angle = 90;
	}
}

void Cat::MoveY(int dir) 
{
	m_rDst.y += dir * m_moveSpeed;
	if (dir > 0)
	{
		angle = 0;
	}
	else if (dir < 0)
	{
		angle = 180;
	}
}

void Cat::TargetPlayer()
{
	SetTargetX(Game::GetInstance()->GetPlayer()->GetX());
	SetTargetY(Game::GetInstance()->GetPlayer()->GetY());
}

void Cat::TargetScatter()
{
}
void Cat::TargetDeath()
{
}
// helper function for seek
void Cat::DistanceNorth()
{
	// if North is not an obstacle and if the direction is the opposite of the checking direction to prevent U-turn
	int temp = 0;
	if (!(Game::GetInstance()->GetLevel()->m_Map[GetY() - 1][GetX()].isObstacle()) && GetDir() != 's')
	{
		temp = 0;// reseting and checking the distance
		temp += (GetTargetX() - GetX()) * (GetTargetX() - GetX());
		temp += (GetTargetY() - (GetY() - 1)) * (GetTargetY() - (GetY() - 1));
		//std::cout << "CAT CHECK NORTH :" << temp << std::endl;
		if (distance > temp)
		{
			distance = temp;
			SetDir('w');
		}
	}
}
// helper function for seek
void Cat::DistanceEast()
{
	// if East is not an obstacle and if the direction is the opposite of the checking direction to prevent U-turn
	if (!(Game::GetInstance()->GetLevel()->m_Map[GetY()][GetX() + 1].isObstacle()) && GetDir() != 'a')
	{
		int temp = 0;
		temp += (GetTargetX() - (GetX() + 1)) * (GetTargetX() - (GetX() + 1));
		temp += (GetTargetY() - GetY()) * (GetTargetY() - GetY());
		//std::cout << "CAT CHECK EAST :" << temp << std::endl;
		if (distance > temp)
		{
			distance = temp;
			SetDir('d');
		}
	}
}
// helper function for seek
void Cat::DistanceSouth()
{
	// if South is not an obstacle and if the direction is the opposite of the checking direction to prevent U-turn
	if (!(Game::GetInstance()->GetLevel()->m_Map[GetY() + 1][GetX()].isObstacle()) && GetDir() != 'w')
	{
		int temp = 0;
		temp += (GetTargetX() - GetX()) * (GetTargetX() - GetX());
		temp += (GetTargetY() - (GetY() + 1)) * (GetTargetY() - (GetY() + 1));
		//std::cout << "CAT CHECK SOUTH :" << temp << std::endl;
		if (distance > temp)
		{
			distance = temp;
			SetDir('s');
		}
	}
}
// helper function for seek
void Cat::DistanceWest()
{
	// if West is not an obstacle and if the direction is the opposite of the checking direction to prevent U-turn
	if (!(Game::GetInstance()->GetLevel()->m_Map[GetY()][GetX() - 1].isObstacle()) && !(GetDir() == 'd'))
	{
		int temp = 0;
		temp += (GetTargetX() - (GetX() - 1)) * (GetTargetX() - (GetX() - 1));
		temp += (GetTargetY() - GetY()) * (GetTargetY() - GetY());
		//std::cout << "CAT CHECK WEST :" << temp << std::endl;
		if (distance > temp)
		{
			distance = temp;
			SetDir('a');
		}
	}
}
void Cat::Seek()
{
	distance = 999999;
	int temp = 0;
	// Check if the square cat is on is an intersection
	if (!IsMoving())
	{
		if (!checkBound) {
			if (Game::GetInstance()->GetLevel()->m_Map[GetY()][GetX()].isIntersection())
			{
				//std::cout << "---------" << std::endl;
				//std::cout << "GOING : " << GetDir() << std::endl;
				//std::cout << "---------" << std::endl;
				checkBound = true;
				for (int dirPriority = 0; dirPriority < 4; dirPriority++)
				{
					switch (GetPriority(dirPriority))
					{
					case 0:
						DistanceNorth();
						break;
					case 1:
						DistanceEast();
						break;
					case 2:
						DistanceSouth();
						break;
					case 3:
						DistanceWest();
						break;
					}
				}
				//std::cout << "---------" << std::endl;
				//std::cout << "shortest : " << GetDir() << " | Distance :" << distance << std::endl;
				//std::cout << "---------" << std::endl;
			}
		}
	}
	// if not moving then move in a direction
	if (!IsDead() && !IsMoving()) {
		if (GetDir() == 'w') {
			// If not an obstacle then sets the new destination square
			if (!Game::GetInstance()->GetLevel()->m_Map[GetY() - 1][GetX()].isObstacle()) {
				SetDestinationY(GetDst().y - 64);
				SetDestinationX(GetDst().x);
				SetMoving(true);
			}
			else // if Obstacle it will check for another route 
			{//need to edit to take in direction priority
				if (!Game::GetInstance()->GetLevel()->m_Map[GetY()][GetX() + 1].isObstacle())
				{
					SetDir('d');
				}
				else if (!Game::GetInstance()->GetLevel()->m_Map[GetY()][GetX() - 1].isObstacle())
				{
					SetDir('a');
				}
			}
		}
		else if (GetDir() == 's') {
			if (!Game::GetInstance()->GetLevel()->m_Map[GetY() + 1][GetX()].isObstacle()) {
				SetDestinationY(GetDst().y + 64);
				SetDestinationX(GetDst().x);
				SetMoving(true);
			}
			else
			{//need to edit to take in direction priority
				if (!Game::GetInstance()->GetLevel()->m_Map[GetY()][GetX() - 1].isObstacle())
				{
					SetDir('a');
				}
				else if (!Game::GetInstance()->GetLevel()->m_Map[GetY()][GetX() + 1].isObstacle())
				{
					SetDir('d');
				}
			}
		}
		else if (GetDir() == 'a') {
			if (!Game::GetInstance()->GetLevel()->m_Map[GetY()][GetX() - 1].isObstacle())
			{
				SetDestinationX(GetDst().x - 64);
				SetDestinationY(GetDst().y);
				SetMoving(true);
			}
			else
			{//need to edit to take in direction priority
				if (!Game::GetInstance()->GetLevel()->m_Map[GetY() - 1][GetX()].isObstacle())
				{
					SetDir('w');
				}
				else if (!Game::GetInstance()->GetLevel()->m_Map[GetY() + 1][GetX()].isObstacle())
				{
					SetDir('s');
				}
			}
		}
		else if (GetDir() == 'd') {
			if (!Game::GetInstance()->GetLevel()->m_Map[GetY()][GetX() + 1].isObstacle()) {
				SetDestinationX(GetDst().x + 64);
				SetDestinationY(GetDst().y);
				SetMoving(true);
			}
			else
			{//need to edit to take in direction priority
				if (!Game::GetInstance()->GetLevel()->m_Map[GetY() - 1][GetX()].isObstacle())
				{
					SetDir('w');
				}
				else if (!Game::GetInstance()->GetLevel()->m_Map[GetY() + 1][GetX()].isObstacle())
				{
					SetDir('s');
				}
			}
		}
	}
	// if moving continue moving till
	if (IsMoving()) {
		Animate();
		if (GetDestinationX() > GetDst().x) {
			MoveX(1);
		}
		else if (GetDestinationX() < GetDst().x) {
			MoveX(-1);
		}
		else if (GetDestinationY() > GetDst().y) {
			MoveY(1);
		}
		else if (GetDestinationY() < GetDst().y) {
			MoveY(-1);
		}
		// if cat has gotten to destination then set moving to false
		else if (GetDestinationX() == GetDst().x && GetDestinationY() == GetDst().y) {
			SetMoving(false);
		}
	}
	if (!Game::GetInstance()->GetLevel()->m_Map[GetY()][GetX()].isIntersection() && checkBound)
	{
		checkBound = false;
	}
}

void Cat::SetVulnerable(bool b)
{
	if (b) {
		m_bIsVulnerable = true;
		if (!m_bIsDead) {
			m_rSrc = { 768, 0, 64, 64 };
		}
	}
	else {
		m_bIsVulnerable = false;
	}
}

void Cat::Update()
{
	TargetPlayer();
	switch (GetState())
	{
	case C_State::IDLE:
		break;
	case C_State::WAKEUP:
		break;
	case C_State::SCATTER:
		TargetScatter();
		Seek();
		break;
	case C_State::SEEK:
		TargetPlayer();
		Seek();
		break;
	case C_State::DEATH:
		TargetDeath();
		Seek();
		break;
	}
}

void Cat::Die() {
	m_bIsDead = true;
	m_rSrc = {};
}