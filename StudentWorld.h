#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include "GameController.h"
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetDir)
		: GameWorld(assetDir)
	{;}

	~StudentWorld() 
	{
		cleanUp();
	}

	virtual int init();

	bool digEarth(int x, int y);

	virtual int move();

	virtual void cleanUp();
	
	void setDisplayText();

	std::ostringstream outputFormatter(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score);

	bool finishedLevel();

	void shootSquirt(int init_x, int init_y, GraphObject::Direction initDirection);

	bool isEarth(int x, int y, GraphObject::Direction dir);

	bool isBoulder(int x, int y, GraphObject::Direction dir);

	void setObjectPosition(int x, int y, char object);

	char getObjectPosition(int x, int y)
	{
		return objectMatrix[x][y];
	};

	bool overlap(int x, int y);

	bool canShout(int currX, int currY, GraphObject::Direction currDir);

	Tunnelman* getTunnelman()
	{
		return m_tunnelman;
	};

	bool facingTunnelman(int currX, int currY, GraphObject::Direction& currDir);

	bool nearTunnelman(int currX, int currY, int dis);

	GraphObject::Direction newDirRandom(int currX, int currY);

	void showHidden(int currX, int currY);

	void dropGold(int currX, int currY);

	bool annoyProtester(int currX, int currY, int damage);

	void annoyTunnelman(int currX, int currY);

	void decreaseBarrel()
	{
		B--;
	}

	Object* findProtester(int currX, int currY);

	void removeDeadGameObjects();

	bool canAddProtester();

	bool canAddWater(int currX, int currY);

	bool playerDied();
	
private:
	int nEarth;
	int B;
	int G;
	int L;
	int nProtesters;
	int ticksSinceLastProtester;
	int ticksForNextProtester;
	std::vector<Object*> objectV;
	Earth* earthArray[3616];
	char objectMatrix[64][64]; // E for Earth, T for Empty, B for Boulder, G for Gold, L for Oil
	Tunnelman* m_tunnelman;
};

#endif // STUDENTWORLD_H_
