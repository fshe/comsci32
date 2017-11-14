#include "actor.h"
#include "StudentWorld.h"
#include <string>
#include <cstdlib>
#include <vector>
//class Actor
Actor::Actor(int imageID, int startX, int startY, StudentWorld* pointer)
:GraphObject(imageID,startX,startY), m_world(pointer)
{m_deadStatus = false;} //not dead
bool Actor::isDead(){return m_deadStatus;}
void Actor::setStatus(bool status)
{
	m_deadStatus = status;
}
StudentWorld* Actor::getWorld()
{return m_world;}



//class Ship
void Ship::launchProjectile(std::string type, bool PlayerFired)
{
	if(PlayerFired ==true)
		getWorld()->insertProjectile(getX(), getY()+1, type, true);
	else getWorld()->insertProjectile(getX(), getY()-1, type, false);
}
int Ship::getEnergy()const {return m_energy;}
void Ship::decreaseEnergy(int points){ 
	m_energy -= points;
}
void Ship::restoreFullEnergy(){ m_energy = m_startEnergy;}
int Ship::getStartEnergy(){return m_startEnergy;}



//class PlayerShip
PlayerShip::PlayerShip(StudentWorld* pointer)
:Ship(IID_PLAYER_SHIP,VIEW_WIDTH/2,1, pointer,50)
{
	m_canIshoot = true;
	m_torpedoes = 0;
}
void PlayerShip::doSomething()
{
	hasCollided();
	if(getEnergy() <=0){
		setStatus(true);
		return;
	}
	int ch;
	if(getWorld()->getKey(ch))
	{//user hit a key this tick!
		switch(ch){
			case KEY_PRESS_LEFT:
				if(getX()>0)
					moveTo(getX()-1,getY());
				m_canIshoot = true;
				break;
			case KEY_PRESS_RIGHT:
				if(getX()<VIEW_WIDTH-1)
					moveTo(getX()+1, getY());
				m_canIshoot = true;
				break;
			case KEY_PRESS_UP:
				if(getY() < VIEW_HEIGHT-1)
					moveTo(getX(), getY()+1);
				m_canIshoot = true;
				break;
			case KEY_PRESS_DOWN:
				if(getY()>0)
					moveTo(getX(), getY()-1);
				m_canIshoot = true;
				break;
			case KEY_PRESS_SPACE://tbc..
				if(!m_canIshoot)
				{m_canIshoot = true;
				break;
				}
				if(getY()!=VIEW_HEIGHT-1 && m_canIshoot == true){
					launchProjectile("BULLET",true);
					getWorld()->playSound(SOUND_PLAYER_FIRE);
					m_canIshoot = false;
				}
				break;
			case KEY_PRESS_TAB:
				if(!m_canIshoot)
				{m_canIshoot = true;
				break;
				}
				if(getY()!=VIEW_HEIGHT-1 && m_canIshoot && m_torpedoes > 0){
					launchProjectile("TORPEDO", true);
					getWorld()->playSound(SOUND_PLAYER_TORPEDO);
					m_canIshoot = false;
					m_torpedoes--;
				}
				break;
		}
	}
	hasCollided();
}

void PlayerShip::damage(int points, bool hitByProjectile)
{
	decreaseEnergy(points);
	if(getEnergy()<=0)
		setStatus(true);
	if(hitByProjectile)
		getWorld()->playSound(SOUND_PLAYER_HIT);
	else
		getWorld()->playSound(SOUND_ENEMY_PLAYER_COLLISION);
	if (isDead())
		getWorld()->playSound(SOUND_PLAYER_DIE);
}
void PlayerShip::addTorpedoes(int n)
{
	m_torpedoes += n;
}
int PlayerShip::numTorpedoes()
{
	return m_torpedoes;
}
void PlayerShip::hasCollided()
{
	vector<Alien*> v = getWorld()->getCollidingAliens(this);
	for(int k=0;k<v.size();k++)
	{
		damage(15,false);
		v[k]->setStatus(true);
	}
} 

//class Alien
Alien::Alien(int imageID, int startX, int startY, StudentWorld* pointer, int m_energy)
	:Ship(imageID,startX,startY,pointer, m_energy)
{}

//class Nachling
Nachling::Nachling(StudentWorld* pointer, int startEnergy)
	:NachlingBase(IID_NACHLING, (rand()%VIEW_WIDTH) ,(VIEW_HEIGHT-1) ,pointer, startEnergy)
{
}
void Nachling::damage(int points,bool hitByProjectile)
{
	if(hitByProjectile)
		decreaseEnergy(points);
	else
		decreaseEnergy(getStartEnergy());
	if(getEnergy() <=0)
	{
		getWorld()->playSound(SOUND_ENEMY_DIE);
		setStatus(true);
		getWorld()->incAliensDestroyed();
		if(hitByProjectile)
			getWorld()->increaseScore(1000);
	}
	else
		getWorld()->playSound(SOUND_ENEMY_HIT);
}


//class NachlingBase
NachlingBase::NachlingBase(int imageID, int startX, int startY, StudentWorld* pointer, int m_energy)
	:Alien(imageID,startX,startY,pointer,m_energy)
{
	m_state=0;
	m_tick=0;
	MDB=0;
	HMD=0;
	HMR=0;

}
void NachlingBase::doSomething()
{
int chancesOfFiring;
	if(m_tick==1)
	{
		m_tick=0;
		return;
	}
	m_tick = 1;

	switch(m_state){
		case 0:
			if(getX() != (getWorld()->player()->getX()))
			{
				int k = rand()%3;
				if(k==0 || k ==1)
				{
				moveTo(getX(),getY()-1);
				if(getY()<0)
					setStatus(true);
				return;
				}

				else
				{
					if(getX()>getWorld()->player()->getX())
					moveTo(getX()-1,getY()-1);
					else if (getX()<getWorld()->player()->getX())
						moveTo(getX()+1, getY()-1);
					else if (getX()==getWorld()->player()->getX())
						moveTo(getX(),getY()-1);
					return;
				}
			}
			else
				{
					if(getX()!=0 && getX()!=VIEW_WIDTH-1)
				{
					m_state = 1;
					if(getX()>(VIEW_WIDTH -1) - getX())
						MDB = (VIEW_WIDTH -1) - getX();
					else
						MDB = getX();
					if(MDB>3)
						HMD = (rand()%3)+1;
					else
						HMD = MDB;
					//0 = left, 1 = right
					movementDirection = rand()%2;
					HMR = HMD;
					}
				}
				moveTo(getX(),getY()-1);
				if(getY()<0)
					setStatus(true);
				break;
		case 1: if((getWorld()->player()->getY()) > getY())
				{
					m_state = 2;
					return;
				}
				if(HMR==0){
					if(movementDirection == 0)
						movementDirection=1;
					else movementDirection =0;
					HMR = 2*HMD;
				}
				else HMR--;
				if (movementDirection == 0)
					moveTo(getX()-1,getY());
				else moveTo(getX()+1, getY());
				chancesOfFiring = int(10/getWorld()->getRound())+1;
				if(rand()%chancesOfFiring ==1)
				{
					if(getWorld()->enemyProjectiles() < 2*getWorld()->getRound())
					{
						launchProjectile("BULLET",false);
						getWorld()->playSound(SOUND_ENEMY_FIRE);
					}
				}
				if(rand()%20 ==0)
					m_state=2;
				break;
		case 2:
			if(getY()==VIEW_HEIGHT-1)
			{
				m_state = 0;
				return;
			}
			if (getX()==0)
				movementDirection = 1;
			if (getX()==VIEW_WIDTH-1)
				movementDirection=0;
			if (getX()>0 && getX() < VIEW_WIDTH-1)
				movementDirection= rand()%2;
			
			if (movementDirection ==0)
				moveTo(getX()-1, getY()+1);
			else moveTo(getX()+1,getY()+1);
			break;
	}

}


//class WealthyNachling
WealthyNachling::WealthyNachling(StudentWorld* pointer, int startEnergy)
	:NachlingBase(IID_WEALTHY_NACHLING, rand()%VIEW_WIDTH ,VIEW_HEIGHT-1 ,pointer, startEnergy)
{
	malfunction = 0;
	m_ticker =0;
}
void WealthyNachling::doSomething()
{
	if(m_ticker ==30)
	{
		m_ticker = 0;
		malfunction = 0;
	}
	if(malfunction ==1){
		m_ticker++;
		return;
	}
	if(malfunction==0){
		if(rand()%200==0)
			malfunction=1;
	}
	if(malfunction ==0) 
		NachlingBase::doSomething();
}
void WealthyNachling::damage(int points,bool hitByProjectile)
{
	if(hitByProjectile)
		decreaseEnergy(points);
	else
		decreaseEnergy(getStartEnergy());
	if(getEnergy() <=0)
	{
		getWorld()->playSound(SOUND_ENEMY_DIE);
		setStatus(true);
		getWorld()->incAliensDestroyed();
		if(hitByProjectile){
			getWorld()->increaseScore(1200);
			if(rand()%3==0)
				if(rand()%2==1)
					getWorld()->insertGoodie(getX(),getY(), "ENERGY");
				else getWorld()->insertGoodie(getX(),getY(), "TORPEDO");
		}
	}
	else
		getWorld()->playSound(SOUND_ENEMY_HIT);
}

//class SmallBot
SmallBot::SmallBot(StudentWorld* pointer, int startEnergy)
	:Alien(IID_SMALLBOT,rand()%VIEW_WIDTH, VIEW_HEIGHT-1,pointer,startEnergy)
{
	m_tick = 0;
	m_justHit = false;
}
void SmallBot::doSomething()
{
	if(m_tick==1)
	{
		m_tick =0;
		return;
	}
	m_tick =1;
	if(m_justHit && (rand()%3==0))
	{
		m_justHit = false;
		if(getX()==0)
			moveTo(1,getY()-1);
		else if(getX()==VIEW_WIDTH-1)
			moveTo(VIEW_WIDTH-2,getY()-1);
		else 
		{
			int k = rand()%2;
			if(k==0)
				moveTo(getX()+1,getY()-1);
			if(k==1)
				moveTo(getX()-1,getY()-1);
		}
	}
	else
	{   m_justHit = false;
	moveTo(getX(),getY()-1);}

	if(getX()==getWorld()->player()->getX() && getY() > getWorld()->player()->getY())
	{
		int q = int(100.0/getWorld()->getRound()) + 1;
		std::string type;
		if(rand()%q ==0)
			type = "TORPEDO";
		else type = "BULLET";

		int counter = getWorld()->enemyProjectiles();
		if(counter < 2*getWorld()->getRound()){
			launchProjectile(type,false);
			getWorld()->playSound(SOUND_ENEMY_FIRE);
		}
	}

	if(getY() <0)
		setStatus(true);
}
void SmallBot::damage(int points, bool hitByProjectile)
{
	m_justHit = true;
	if(hitByProjectile)
		decreaseEnergy(points);
	else
		decreaseEnergy(getStartEnergy());
	if(getEnergy() <=0)
	{
		getWorld()->playSound(SOUND_ENEMY_DIE);
		setStatus(true);

		getWorld()->incAliensDestroyed();
		if(hitByProjectile){
			getWorld()->increaseScore(1500);
			if(rand()%3==0)
				getWorld()->insertGoodie(getX(),getY(), "SHIP");
	}
	else
		getWorld()->playSound(SOUND_ENEMY_HIT);
}
}
//class Star
Star::Star(StudentWorld* pointer)
	:Actor(IID_STAR, rand() % VIEW_WIDTH, VIEW_HEIGHT-1, pointer) //-1?
{}
void Star::doSomething()
{
	moveTo(getX(),getY()-1);
	if(getY()<0)
		setStatus(true);
}

//class Projectile
Projectile::Projectile(int imageID, int startX, int startY, StudentWorld* pointer,bool playerFired, int damagePoints)
	:Actor(imageID, startX,startY,pointer)
{
	m_playerShot = playerFired;
	m_damage = damagePoints;
}
bool Projectile::playerFired() const
{return m_playerShot;}
void Projectile::doSomething() //tbc...
{
	hasCollided();
	if(!isDead())
	{
		if(playerFired())
		moveTo(getX(),getY()+1);
		else
			moveTo(getX(),getY()-1);
	}
	hasCollided();
}

void Projectile::hasCollided()
{
	if (playerFired())
	{
		vector<Alien*> collisions = getWorld()->getCollidingAliens(this);
		for(int k=0; k<collisions.size();k++){
			//Nachling* n = dynamic_cast<Nachling*>(collisions[k]);
			//WealthyNachling* w = dynamic_cast<WealthyNachling*>(collisions[k]);
			//SmallBot* s = dynamic_cast<SmallBot*>(collisions[k]);
			//if statements to pass a damage amount!
			if(!collisions[k]->isDead())
				collisions[k]->damage(m_damage,true);
		}
		if(collisions.size()>0)
			setStatus(true);
	}
	else
	{
		PlayerShip* collision = getWorld()->collisionPlayerShip(this);
		if(collision!=NULL){
			if(!collision->isDead())
			{collision->damage(m_damage, true);
			setStatus(true);}
		}
	}
}
//class Bullet
Bullet::Bullet(int startX, int startY, StudentWorld* pointer, bool playerFired)
	:Projectile(IID_BULLET, startX,startY, pointer, playerFired, 2) //check damagepoints
{} 
//class Torpedo
Torpedo::Torpedo(int startX, int startY, StudentWorld* pointer, bool playerFired) //check playerFired
	:Projectile(IID_TORPEDO, startX, startY, pointer, playerFired, 8)
{}

//class Goodie
Goodie::Goodie(int image, int startX, int startY, StudentWorld* pointer)
	:Actor(image, startX, startY, pointer)
{
	m_lifeTime = int(100.0/getWorld()->getRound())+30;
	m_expiredTime=0;
}
bool Goodie::hasCollided()
{
	PlayerShip* collision = getWorld()->collisionPlayerShip(this);
		if(collision!=NULL){
			setStatus(true);
			return true;
		}
	return false;
}
void Goodie::doSomething()
{
	m_expiredTime++;
	if(!hasCollided())
	{
		setBrightness((double(m_lifeTime-m_expiredTime)/double(m_lifeTime))+0.2);
		if(m_lifeTime==m_expiredTime){
			setStatus(true);
			return;
		}
		if(m_expiredTime % 3 == 0)
			moveTo(getX(),getY()-1);
		if(getY()<0)
			setStatus(true);
		if(!hasCollided())
			return;
	}
	getWorld()->increaseScore(5000);
	getWorld()->playSound(SOUND_GOT_GOODIE);
	setStatus(true);
	doSpecialAction();
}
//class FreeShipGoodie
FreeShipGoodie::FreeShipGoodie(int x, int y, StudentWorld* pointer)
	:Goodie(IID_FREE_SHIP_GOODIE, x ,y ,pointer)
{}

void FreeShipGoodie::doSpecialAction()
{getWorld()->incLives();}
//class Energygoodie
EnergyGoodie::EnergyGoodie(int x, int y, StudentWorld* pointer)
	:Goodie(IID_ENERGY_GOODIE, x ,y ,pointer)
{}
void EnergyGoodie::doSpecialAction()
{	getWorld()->player()->restoreFullEnergy();}
//class TorpedoGoodie
TorpedoGoodie::TorpedoGoodie(int x, int y, StudentWorld* pointer)
	:Goodie(IID_TORPEDO_GOODIE, x ,y ,pointer)
{}
void TorpedoGoodie::doSpecialAction()
{	getWorld()->player()->addTorpedoes(5);}
