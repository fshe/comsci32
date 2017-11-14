#ifndef _ACTOR_H_
#define _ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <string>
class StudentWorld;
//Actor
class Actor :public GraphObject
{
public:
	Actor(int imageID, int startX, int startY, StudentWorld* pointer);
	virtual void doSomething()=0;
	StudentWorld* getWorld();
	bool isDead();
	void setStatus(bool status);
private:
	StudentWorld* m_world;
	bool m_deadStatus; //true if dead , false
};

//Ship
class Ship: public Actor
{
public:
	Ship(int imageID, int startX, int startY, StudentWorld* pointer, int startEnergy):Actor(imageID, startX,startY,pointer)
	   { m_energy =startEnergy; m_startEnergy=startEnergy;}
	void launchProjectile(std::string type, bool playerFired);
    int getEnergy() const;
    void decreaseEnergy(int points);
    void restoreFullEnergy();
	int getStartEnergy();
	
private:
	int m_energy;
	int m_startEnergy;
};

class PlayerShip :public Ship
{
public:
	PlayerShip(StudentWorld* pointer);
	virtual void doSomething();
	void damage(int points, bool hitByProjectile);
	void addTorpedoes(int n);
	int numTorpedoes();
	void hasCollided();
private:
	bool m_canIshoot;
	int m_torpedoes;
};

class Alien: public Ship
{
public:
	Alien(int imageID, int startX, int startY, StudentWorld* pointer, int m_energy);
	virtual void damage(int points, bool hitByProjectile)=0;
};

class NachlingBase:public Alien
{
public:
	NachlingBase(int imageID, int startX, int startY, StudentWorld* pointer, int m_energy);
	virtual void doSomething();
private:
	int m_state;
	int m_tick;
	int MDB;
	int HMD;
	int HMR;
	int movementDirection; //left = 0, right =1
};

class Nachling :public NachlingBase
{
public:
	Nachling(StudentWorld* pointer, int startEnergy);
	virtual void damage(int points, bool hitByProjectile);
};



class WealthyNachling:public NachlingBase
{
public:
	WealthyNachling(StudentWorld* pointer, int startEnergy);
	virtual void doSomething();
	virtual void damage(int points, bool hitByProjectile);
private:
	int malfunction; //if 0 then malfunctioning, if 1 not
	int m_ticker;
};




class SmallBot :public Alien
{
public:
	SmallBot(StudentWorld* pointer, int startEnergy);
	virtual void doSomething();
	virtual void damage(int points, bool hitByProjectile);
private:
	int m_tick; //0 okay do act, 1 not allowed
	bool m_justHit; 
};

//Projectile
class Projectile :public Actor
{
public:
	Projectile(int imageID, int startX, int startY, StudentWorld* pointer, bool playerFired, int damagePoints);
	bool playerFired() const;
	virtual void doSomething();
	void hasCollided();
private:
	bool m_playerShot;
	int m_damage;
};

class Bullet :public Projectile
{
public:
	Bullet(int startX, int startY, StudentWorld* pointer, bool playerFired);
};

class Torpedo :public Projectile
{
public:
	Torpedo(int startX, int startY, StudentWorld* pointer, bool playerFired);
};

//Goodie
class Goodie: public Actor
{
public:
	Goodie(int image, int startX, int startY, StudentWorld* pointer);
	virtual void doSpecialAction() = 0;
	virtual void doSomething();
	virtual bool hasCollided();
private:
	int m_lifeTime;
	int m_expiredTime;
};

class FreeShipGoodie:public Goodie
{
public:
	FreeShipGoodie(int x, int y, StudentWorld* pointer);
	virtual void doSpecialAction();
};

class EnergyGoodie: public Goodie
{
public:
	EnergyGoodie(int x, int y, StudentWorld* pointer);
	virtual void doSpecialAction();
};

class TorpedoGoodie: public Goodie
{
public:
	TorpedoGoodie(int x, int y, StudentWorld* pointer);
	virtual void doSpecialAction();
};

//Star
class Star :public Actor
{
public:
	Star(StudentWorld* pointer);
	virtual void doSomething();
};

#endif // _ACTOR_H_