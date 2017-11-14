#ifndef _STUDENTWORLD_H_
#define _STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"

#include "actor.h"
#include <vector>
#include <string>
using namespace std;

class PlayerShip;
class Actor;
class Alien; //need to implement this
class StudentWorld : public GameWorld
{
public:
	StudentWorld();
	virtual ~StudentWorld();
	void addActor();
	void updateDisplayText();
	void removeDeadObjects();
	void insertProjectile(int x, int y, std::string type, bool playerFired); 
	void insertGoodie(int x, int y, std::string type);
	int getRound();
	int enemyProjectiles();
	virtual void init();
	virtual int move();
	virtual void cleanUp();
	PlayerShip* player();
	PlayerShip* collisionPlayerShip(Actor* a);
	std::vector<Alien*> getCollidingAliens(Actor *a);
	void incRound();
	int retRound();
	void incAliensDestroyed();
	int maxAliens();
	int aliensNeeded();
	int aliensDestroyed();
private:
 PlayerShip* m_pShip;//vectors
 vector<Actor*> m_vActor; //remember to pop from this
 int m_round;
 int m_aliensDestroyed;
};

#endif // _GAMEWORLD_H_
