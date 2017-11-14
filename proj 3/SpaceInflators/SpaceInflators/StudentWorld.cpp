#include "StudentWorld.h"
#include "actor.h"
#include <iostream>
#include <sstream>
#include <iomanip>
GameWorld* createStudentWorld()
{
    return new StudentWorld();
}
StudentWorld::StudentWorld()
	:m_round(1),m_aliensDestroyed(0) //will this mess this up?
{
}
StudentWorld::~StudentWorld()
{
	delete m_pShip;
	for(int k=0;k<m_vActor.size();k++){
		delete m_vActor[k];
		m_vActor.erase(m_vActor.begin()+k);
		k--;
	}
}
void StudentWorld::incRound(){m_round++; m_aliensDestroyed=0;}
int StudentWorld::retRound(){return m_round;}
void StudentWorld::incAliensDestroyed(){m_aliensDestroyed++;}
int StudentWorld::maxAliens(){return (2+.5*getRound());}
int StudentWorld::aliensNeeded(){return (4*getRound()-aliensDestroyed());}
int StudentWorld::aliensDestroyed(){return m_aliensDestroyed;}
void StudentWorld::addActor() //Aliens and Stars
{
	double RoundFactor = 0.9 + 01 *getRound();
	if(testParamsProvided())
	{
		bool atLeastOne = false;
		for(int k=0;k<m_vActor.size();k++){
			Alien* p = dynamic_cast<Alien*>(m_vActor[k]);
			Goodie* s = dynamic_cast<Goodie*>(m_vActor[k]);
			if(p !=NULL || s!=NULL)
				atLeastOne=true;
		}
		if(atLeastOne)
			return;
		int whichActor = getTestParam(TEST_PARAM_ACTOR_INDEX);
		if(whichActor == TEST_PARAM_NACHLING)
		{
			m_vActor.push_back(new Nachling(this,(5*RoundFactor)));
			m_vActor[m_vActor.size()-1]->setVisible(true);
		}
		if(whichActor == TEST_PARAM_WEALTHY_NACHLING)
		{
			m_vActor.push_back(new WealthyNachling(this,(8*RoundFactor)));
			m_vActor[m_vActor.size()-1]->setVisible(true);
		}
		if(whichActor == TEST_PARAM_SMALLBOT)
		{
			m_vActor.push_back(new SmallBot(this,(12*RoundFactor)));
			m_vActor[m_vActor.size()-1]->setVisible(true);
		}
		if(whichActor == TEST_PARAM_GOODIE_ENERGY)
		{
			m_vActor.push_back(new EnergyGoodie(VIEW_WIDTH/2,VIEW_HEIGHT-1,this));
			m_vActor[m_vActor.size()-1]->setVisible(true);
		}
		if(whichActor == TEST_PARAM_GOODIE_TORPEDO)
		{
			m_vActor.push_back(new TorpedoGoodie(VIEW_WIDTH/2,VIEW_HEIGHT-1,this));
			m_vActor[m_vActor.size()-1]->setVisible(true);
		}
		if(whichActor == TEST_PARAM_GOODIE_FREE_SHIP)
		{
			m_vActor.push_back(new FreeShipGoodie(VIEW_WIDTH/2,VIEW_HEIGHT-1,this));
			m_vActor[m_vActor.size()-1]->setVisible(true);
		}
		return;
	}
	else{
		int counter =0;
		for(int k=0;k<m_vActor.size();k++){
			Alien* p = dynamic_cast<Alien*>(m_vActor[k]);
			if(p !=NULL)
				counter++;}
		if(!(counter >= maxAliens()) && !(counter >= aliensNeeded()))
		{
			int p1 = rand()%101;
			if(p1 < 70)
			{
				int p2 = rand()%101;
				if(p2<20){
					m_vActor.push_back(new WealthyNachling(this,(8*RoundFactor)));
					m_vActor[m_vActor.size()-1]->setVisible(true);
				}
				else
				{
					m_vActor.push_back(new Nachling(this,(5*RoundFactor)));
					m_vActor[m_vActor.size()-1]->setVisible(true);
				}
			}
			else
			{
				m_vActor.push_back(new SmallBot(this,(12*RoundFactor)));
				m_vActor[m_vActor.size()-1]->setVisible(true);
			}
		}
	if (rand() % 3 == 0) //1/3 chance
	{
		m_vActor.push_back(new Star(this));
		m_vActor[m_vActor.size()-1]->setVisible(true);
	}
	return;
	}
}
void StudentWorld::updateDisplayText()
{
	int score = getScore();
	int round = getRound();
	int energyPercent = m_pShip->getEnergy() *2;
	int torpedoes = m_pShip->numTorpedoes();
	int shipsLeft = getLives();
	ostringstream oss;
    oss.fill('0');
	oss << "Score: "; 
	oss << setw(7) << score;
	oss	<< "  Round: " << setw(2) <<round;
	oss	<< "  Energy: " <<(energyPercent)<<"%";
	oss	<< "  Torpedoes: " << setw(3) << torpedoes; 
	oss	<< "  Ships: " << setw(2) << shipsLeft;
	string s = oss.str();
	setGameStatText(s);
}
void StudentWorld::removeDeadObjects()
{
	//just for stars here
	//should we remove player ship?
	for(int k=0; k<m_vActor.size();k++)
		if(m_vActor[k]->isDead()||m_vActor[k]->getY()<0 || m_vActor[k]->getY()>VIEW_HEIGHT-1 ||m_vActor[k]->getX()<0 ||m_vActor[k]->getX()>VIEW_WIDTH-1 ){
			delete m_vActor[k];
			m_vActor.erase(m_vActor.begin()+k); //check this code!
			k--;
		}
}
void StudentWorld::insertProjectile(int x, int y, std::string type, bool playerFired)
{
	if(type == "BULLET")
	{
		m_vActor.push_back(new Bullet(x,y,this, playerFired));
		m_vActor[m_vActor.size()-1]->setVisible(true);
	}
	if(type=="TORPEDO")
	{
		m_vActor.push_back(new Torpedo(x,y,this,playerFired));
		m_vActor[m_vActor.size()-1]->setVisible(true);
	}
}
void StudentWorld::insertGoodie(int x, int y, std::string type)
{
	if(type == "ENERGY")
	{
		m_vActor.push_back(new EnergyGoodie(x,y,this));
		m_vActor[m_vActor.size()-1]->setVisible(true);
	}
		if(type == "TORPEDO")
	{
		m_vActor.push_back(new TorpedoGoodie(x,y,this));
		m_vActor[m_vActor.size()-1]->setVisible(true);
	}
	if(type == "SHIP")
	{
		m_vActor.push_back(new FreeShipGoodie(x,y,this));
		m_vActor[m_vActor.size()-1]->setVisible(true);
	}

}
int StudentWorld::getRound()
{return m_round;}
int StudentWorld::enemyProjectiles()
{
	int count = 0;
	for(int k=0; k<m_vActor.size(); k++)
	{
		Projectile* p = dynamic_cast<Projectile*>(m_vActor[k]);
			if(p !=NULL)
				if(!p->playerFired())
					count++;
	} //end for
	return count;
}
void StudentWorld::init()
{
	m_pShip = new PlayerShip(this);
	m_pShip->setVisible(true);
}
int StudentWorld::move()
{
	addActor();
	updateDisplayText();
	for(int k = 0; k<m_vActor.size();k++)
	{
		if(!m_vActor[k]->isDead())
			m_vActor[k]->doSomething();
	}
	if(!m_pShip->isDead())
		m_pShip->doSomething();
	removeDeadObjects();
	if(aliensNeeded() == 0)
		incRound();
	if(m_pShip->isDead()){
		decLives();
		return GWSTATUS_PLAYER_DIED;
	}
	return GWSTATUS_CONTINUE_GAME;
}
void StudentWorld::cleanUp()
{
	delete m_pShip;
	for(int k=0;k<m_vActor.size();k++){
		delete m_vActor[k];
		m_vActor.erase(m_vActor.begin()+k);
		k--;
	}
}

PlayerShip* StudentWorld::player()
{
	PlayerShip* c = m_pShip;
	return c;
}
PlayerShip* StudentWorld::collisionPlayerShip(Actor* a)
{
	if (m_pShip->getX() == a->getX() && m_pShip->getY() == a->getY())
		return m_pShip;
	else return NULL;
}
std::vector<Alien*> StudentWorld::getCollidingAliens(Actor *a)
{
	vector<Alien*> returnMe; //hmmmmmm destruction of this..? all these pointers..
	for(int k=0;k<m_vActor.size();k++)
		if(a->getX()==m_vActor[k]->getX() && a->getY()==m_vActor[k]->getY())
		{
			Alien* p = dynamic_cast<Alien*>(m_vActor[k]);
			if(p !=NULL)
				returnMe.push_back(p);
		}
	return returnMe; //check this function
}