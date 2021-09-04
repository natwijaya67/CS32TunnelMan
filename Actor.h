#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <queue>


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;

struct Point
{
	Point(int x, int y) : m_x(x), m_y(y) { ; }
	int m_x;
	int m_y;
};

class Object :public GraphObject
{
public:
	Object(int imageID, int initX, int initY, Direction initDirection, double size, unsigned int depth, StudentWorld* world)
		:GraphObject(imageID, initX, initY, initDirection, size, depth)
	{
		m_world = world;
		m_alive = true;
	}

	virtual ~Object() { ; }

	virtual void doSomething() = 0;

	StudentWorld* getWorld()
	{
		return m_world;
	}

	bool isAlive()
	{
		return m_alive;
	}

	void setDead()
	{
		m_alive = false;
	}

	virtual bool isProtester()
	{
		return false;
	}

	virtual std::string getState() { return ""; }

	// For persons classes

	virtual void loseHealth(int damage) { ; }

	virtual void getBribed() { ; }

private:
	bool m_alive;
	StudentWorld* m_world;
};

class Earth : public Object
{
public:
	Earth(int initX, int initY, StudentWorld* world)
		:Object(TID_EARTH, initX, initY, right, 0.25, 3, world)
	{
		setVisible(true);
	}

	virtual ~Earth() { ; }

	virtual void doSomething(){ ; }
};

class Squirt : public Object
{
public:
	Squirt(int init_x, int init_y, Direction initDirection, StudentWorld* world)
		:Object(TID_WATER_SPURT, init_x, init_y, initDirection, 1.0, 1, world)
	{
		travelD = 4;
		setVisible(true);
	}

	virtual void doSomething();

private:
	int travelD;
};

class Boulder : public Object
{
public:
	Boulder(int init_x, int init_y, StudentWorld* world)
		:Object(TID_BOULDER, init_x, init_y, down, 1.0, 1, world)
	{
		m_state = "stable";
		setVisible(true);
	}

	virtual void doSomething();

private:
	std::string m_state;
	int m_waitTicks;
};

// Person and Others
class Person : public Object
{
public:
	Person(int imageID, int initX, int initY, Direction initDirection, double size, unsigned int depth, StudentWorld* world)
		:Object(imageID, initX, initY, initDirection, size, depth, world)
	{
		setVisible(true);
	}

	void setHP(int hp)
	{
		m_hp = hp;
	}

	int getHP()
	{
		return m_hp;
	}

	virtual void loseHealth(int damage)
	{
		m_hp -= damage;
		if (m_hp <= 0)
		{
			giveUp();
		}
	}

	virtual void giveUp() = 0;

private:
	int m_hp;
};

class Tunnelman : public Person
{
public:
	Tunnelman(StudentWorld* world)
		:Person(TID_PLAYER, 30, 60, right, 1.0, 0, world)
	{
		setHP(10);
		m_water = 5;
		m_gold = 0;
		m_sonar = 1;
	}

	~Tunnelman() { ; }

	int currentSquirts()
	{
		return m_water;
	}

	int currentGold()
	{
		return m_gold;
	}

	int currentSonar()
	{
		return m_sonar;
	}

	void addSquirts()
	{
		m_water += 5;
	}

	void addGold()
	{
		m_gold++;
	}

	void addKit()
	{
		m_sonar++;
	}

	virtual void doSomething();

	virtual void giveUp();

private:
	int m_water;
	int m_sonar;
	int m_gold;
};

class Protester : public Person
{
public:
	Protester(int imageID, StudentWorld* world);

	std::string getState()
	{
		return m_state;
	}

	void setLeaveState()
	{
		m_state = "leave-the-oil-field";
		m_restTicksLeft = 0;
	}

	virtual bool isProtester()
	{
		return true;
	}

	void doNothing()
	{
		m_restTicksLeft--;
		m_ticksSinceShout++;
		m_ticksSinceTurn++;
	}

	int getRestTicksLeft()
	{
		return m_restTicksLeft;
	}

	int getTicksSinceShout()
	{
		return m_ticksSinceShout;
	}

	void resetShoutTicks()
	{
		m_ticksSinceShout = 0;
	}

	void increaseShoutTicks()
	{
		m_ticksSinceShout++;
	}

	void resetRestTicks()
	{
		m_restTicksLeft = m_ticksToWaitBetweenMoves;
	}

	bool findPath(int endX, int endY, int initX, int initY, Direction& nextStep, int& steps);

	void shout();

	void resetNumSquaresToMoveInCurrDir()
	{
		m_numSquaresToMoveInCurrDir = 0;
	}

	void decreaseNumSquaresToMoveInCurrDir()
	{
		m_numSquaresToMoveInCurrDir--;
	}

	int getNumSquaresToMoveInCurrDir()
	{
		return m_numSquaresToMoveInCurrDir;
	}

	void setNumSquaresToMoveInCurrDir()
	{
		m_numSquaresToMoveInCurrDir = rand() % 53 + 8;
	}

	int getTicksSinceTurn()
	{
		return m_ticksSinceTurn;
	}

	void resetTurnTicks()
	{
		m_ticksSinceTurn = 0;
	}

	void increaseTurnTicks()
	{
		m_ticksSinceTurn++;
	}

	virtual void loseHealth(int damage);

	void getStunned();

	virtual bool isHardcore()
	{
		return false;
	}

	virtual void giveUp();

	void endTurn()
	{
		m_restTicksLeft--;
		m_ticksSinceShout++;
		m_ticksSinceTurn++;
	}

private:
	int m_ticksToWaitBetweenMoves;
	int m_restTicksLeft;
	int m_numSquaresToMoveInCurrDir;
	int m_ticksSinceShout;
	int m_ticksSinceTurn;
	std::string m_state;
};

class RegularProtester : public Protester {
public:
	RegularProtester(StudentWorld* world)
		:Protester(TID_PROTESTER, world)
	{
		setHP(5);
	}

	virtual void doSomething();

	virtual void getBribed();
};

class HardcoreProtester : public Protester {
public:
	HardcoreProtester(StudentWorld* world)
		:Protester(TID_HARD_CORE_PROTESTER, world)
	{
		setHP(20);
	}

	virtual bool isHardcore()
	{
		return true;
	}

	virtual void doSomething();

	virtual void getBribed();
};

// Goodies and Others
class Goodies : public Object
{
public:
	Goodies(int imageID, int initX, int initY, Direction initDirection, double size, unsigned int depth, StudentWorld* world);

	void setVisible(bool visibility)
	{
		GraphObject::setVisible(visibility);
		m_visible = visibility;
	}

	bool is_visible()
	{
		return m_visible;
	}

	void setPickup(bool pickup)
	{
		m_tunnelpickup = pickup;
	}

	bool can_pickup()
	{
		return m_tunnelpickup;
	}

	void setRestTicks(int amt)
	{
		m_restTicksLeft = amt;
	}

	int getRestTicks()
	{
		return m_restTicksLeft;
	}

	void reduceRestTicks()
	{
		m_restTicksLeft--;
	}

private:
	bool m_visible;
	bool m_tunnelpickup;
	int m_restTicksLeft;
};

class Sonar : public Goodies
{
public:
	Sonar(StudentWorld* world)
		:Goodies(TID_SONAR, 0, 60, right, 1.0, 2, world)
	{
		m_state = "temporary";
		setPickup(true);
		setVisible(true);
	}

	virtual void doSomething();

private:
	std::string m_state;
};

class Water : public Goodies
{
public:
	Water(int init_x, int init_y, StudentWorld* world)
		:Goodies(TID_WATER_POOL, init_x, init_y, right, 1.0, 2, world)
	{
		m_state = "temporary";
		setPickup(true);
		setVisible(true);
	}

	virtual void doSomething();

private:
	std::string m_state;
};

class Oil : public Goodies
{
public:
	Oil(int init_x, int init_y, StudentWorld* world)
		:Goodies(TID_BARREL, init_x, init_y, right, 1.0, 2, world)
	{
		setVisible(false);
	}

	virtual void doSomething();

};

class Gold : public Goodies
{
public:
	Gold(int init_x, int init_y, bool isVisible, bool pickup, std::string state, StudentWorld* world)
		:Goodies(TID_GOLD, init_x, init_y, right, 1.0, 2, world)
	{
		setVisible(isVisible);
		setPickup(pickup);
		m_state = state;
		setRestTicks(100);
	}

	virtual void doSomething();

private:
	std::string m_state;
};

#endif // ACTOR_H_