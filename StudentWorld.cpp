#include "StudentWorld.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <iomanip>

using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Functions used in Actor cpp

bool StudentWorld::nearTunnelman(int currX, int currY, int dist)
{
	int dSquared = pow((m_tunnelman->getX() - currX), 2) + pow((m_tunnelman->getY() - currY), 2);
	if (sqrt(dSquared) <= dist) return true;
	else return false;
}

void StudentWorld::annoyTunnelman(int currX, int currY)
{
	if (m_tunnelman->getX() >= currX - 3 && m_tunnelman->getX() <= currX + 3 && m_tunnelman->getY() >= currY - 3 && m_tunnelman->getY() <= currY + 3)
		m_tunnelman->giveUp();
}

bool StudentWorld::annoyProtester(int currX, int currY, int damage)
{
	bool output = false;
	vector<Object*>::iterator it;
	it = objectV.begin();
	while (it != objectV.end())
	{
		if ((*it)->getX() >= currX - 3 && (*it)->getX() <= currX + 3 && (*it)->getY() >= currY - 3 && (*it)->getY() <= currY + 3)
			if ((*it)->isProtester() && (*it)->getState() != "leave-oil-field")
			{
				(*it)->loseHealth(damage);
				output = true;
			}
		it++;
	}

	return output;
}

void StudentWorld::shootSquirt(int init_x, int init_y, GraphObject::Direction initDirection)
{
	objectV.push_back(new Squirt(init_x, init_y, initDirection, this));
	GameController::getInstance().playSound(SOUND_PLAYER_SQUIRT);
}

void StudentWorld::showHidden(int currX, int currY)
{
	vector<Object*>::iterator it;

	it = objectV.begin();

	while (it != objectV.end())
	{
		int dSquared = pow(((*it)->getX() - currX), 2) + pow(((*it)->getY() - currY), 2);
		if (sqrt(dSquared) <= 12)
			(*it)->setVisible(true);
		it++;
	}
	GameController::getInstance().playSound(SOUND_SONAR);
}

bool StudentWorld::isBoulder(int x, int y, GraphObject::Direction dir)
{
	if (dir == GraphObject::down)
	{
		if (getObjectPosition(x, y) == 'B' || getObjectPosition(x + 3, y) == 'B')
			return true;
	}
	else if (dir == GraphObject::up)
	{
		if (getObjectPosition(x, y + 3) == 'B' || getObjectPosition(x + 3, y + 3) == 'B')
			return true;
	}
	else if (dir == GraphObject::left)
	{
		if (getObjectPosition(x, y) == 'B' || getObjectPosition(x, y + 3) == 'B')
			return true;
	}
	else if (dir == GraphObject::right)
	{
		if (getObjectPosition(x + 3, y) == 'B' || getObjectPosition(x + 3, y + 3) == 'B')
			return true;
	}

	return false;
}

bool StudentWorld::isEarth(int x, int y, GraphObject::Direction dir)
{
	if (dir == GraphObject::down)
	{
		for (int i = 0; i < nEarth; i++)
		{
			if (earthArray[i] != nullptr)
			{
				if (earthArray[i]->getX() >= x && earthArray[i]->getX() <= x + 3 && earthArray[i]->getY() == y) return true;
			}
		}
	}
	else if (dir == GraphObject::up)
	{
		for (int i = 0; i < nEarth; i++)
		{
			if (earthArray[i] != nullptr)
			{
				if (earthArray[i]->getX() >= x && earthArray[i]->getX() <= x + 3 && earthArray[i]->getY() == y + 3) return true;
			}
		}
	}
	else if (dir == GraphObject::left)
	{
		for (int i = 0; i < nEarth; i++)
		{
			if (earthArray[i] != nullptr)
			{
				if (earthArray[i]->getX() == x && earthArray[i]->getY() >= y && earthArray[i]->getY() <= y + 3) return true;
			}
		}
	}
	else if (dir == GraphObject::right)
	{
		for (int i = 0; i < nEarth; i++)
		{
			if (earthArray[i] != nullptr)
			{
				if (earthArray[i]->getX() == x + 3 && earthArray[i]->getY() >= y && earthArray[i]->getY() <= y + 3) return true;
			}
		}
	}

	return false;
}

bool StudentWorld::canShout(int currX, int currY, GraphObject::Direction currDir)
{
	if (currDir == GraphObject::up)
	{
		if (currX < m_tunnelman->getX() + 4 && currX > m_tunnelman->getX() - 4 && currY >= m_tunnelman->getY() - 4 && currY <= m_tunnelman->getY())
		{
			return true;
		}
	}
	else if (currDir == GraphObject::down)
	{
		if (currX < m_tunnelman->getX() + 4 && currX > m_tunnelman->getX() - 4 && currY <= m_tunnelman->getY() + 4 && currY >= m_tunnelman->getY())
		{
			return true;
		}
	}
	else if (currDir == GraphObject::left)
	{
		if (currX >= m_tunnelman->getX() && currX <= m_tunnelman->getX() + 4 && currY > m_tunnelman->getY() - 4 && currY < m_tunnelman->getY() + 4)
		{
			return true;
		}
	}
	else if (currDir == GraphObject::right)
	{
		if (currX <= m_tunnelman->getX() && currX >= m_tunnelman->getX() - 4 && currY > m_tunnelman->getY() - 4 && currY < m_tunnelman->getY() + 4)
		{
			return true;
		}
	}
	return false;
}

GraphObject::Direction StudentWorld::newDirRandom(int currX, int currY)
{
	int newDir = rand() % 4 + 1;
	switch (newDir) 
	{
		case 1:
			if (isEarth(currX, currY + 1, GraphObject::up) || isBoulder(currX, currY + 1, GraphObject::up) || currY == 60)
				return newDirRandom(currX, currY);
			else
				return GraphObject::up;
			break;

		case 2:
			if (isEarth(currX, currY - 1, GraphObject::down) || isBoulder(currX, currY - 1, GraphObject::down) || currY == 0)
				return newDirRandom(currX, currY);
			else
				return GraphObject::down;
			break;

		case 3:
			if (isEarth(currX - 1, currY, GraphObject::left) || isBoulder(currX - 1, currY, GraphObject::left) || currX == 0)
				return newDirRandom(currX, currY);
			else
				return GraphObject::left;
			break;

		case 4:
			if (isEarth(currX + 1, currY, GraphObject::right) || isBoulder(currX + 1, currY, GraphObject::right) || currX == 60)
				return newDirRandom(currX, currY);
			else
				return GraphObject::right;
			break;
	}

	return GraphObject::none;
}

bool StudentWorld::facingTunnelman(int currX, int currY, GraphObject::Direction& nextDir)
{
	bool output = false;
	if (!(nearTunnelman(currX, currY, 4)))
	{
		if (currX >= m_tunnelman->getX() - 63 && currX < m_tunnelman->getX() - 4 && currY < m_tunnelman->getY() + 4 && currY > m_tunnelman->getY() - 4)
		{
			output = true;
			nextDir = GraphObject::right;
			for (int i = currX; i != m_tunnelman->getX(); i++)
				if (isEarth(i, currY, GraphObject::right) || isBoulder(i, currY, GraphObject::right))
					output = false;
		}
		else if (currX <= m_tunnelman->getX() + 63 && currX > m_tunnelman->getX() + 4 && currY < m_tunnelman->getY() + 4 && currY > m_tunnelman->getY() - 4)
		{
			output = true;
			nextDir = GraphObject::left;
			for (int i = currX; i != m_tunnelman->getX(); i--)
				if (isEarth(i, currY, GraphObject::left) || isBoulder(i, currY, GraphObject::left))
					output = false;
		}

		else if (currX < m_tunnelman->getX() + 4 && currX > m_tunnelman->getX() - 4 && currY >= m_tunnelman->getY() - 63 && currY < m_tunnelman->getY() - 4)
		{
			output = true;
			nextDir = GraphObject::up;
			for (int i = currY; i != m_tunnelman->getY(); i++)
				if (isEarth(currX, i, GraphObject::up) || isBoulder(currX, i, GraphObject::up))
					output = false;
		}

		else if (currX < m_tunnelman->getX() + 4 && currX > m_tunnelman->getX() - 4 && currY <= m_tunnelman->getY() + 63 && currY > m_tunnelman->getY() + 4)
		{
			output = true;
			nextDir = GraphObject::down;
			for (int i = currY; i != m_tunnelman->getY(); i--)
				if (isEarth(currX, i, GraphObject::down) || isBoulder(currX, i, GraphObject::down))
					output = false;
		}
	}

	return output;
}

Object* StudentWorld::findProtester(int currX, int currY)
{
	vector<Object*>::iterator it;
	it = objectV.begin();

	while (it != objectV.end())
	{
		if ((*it)->isProtester())
		{
			int dSquared = pow(((*it)->getX() - currX), 2) + pow(((*it)->getY() - currY), 2);
			if (sqrt(dSquared) <= 3)
				return (*it);
		}
		it++;
	}

	return nullptr;
}

void StudentWorld::dropGold(int currX, int currY)
{
	objectV.push_back(new Gold(currX, currY, true, false, "temporary", this));
}

// INIT and OTHERS
int StudentWorld::init()
{	
	// Initialize earth in world
	nEarth = 3616;
	int i = 0;
	while (i < nEarth)
	{
		for (int x = 0; x < 64; x++)
		{
			for (int y = 0; y < 60; y++)
			{
				if (x >= 30 && x <= 33 && y > 3)
				{
					earthArray[i] = nullptr;
				}
				else
				{
					earthArray[i] = new Earth(x, y, this);
					objectMatrix[x][y] = 'E';
					i++;
				}
			}
		}
	}

	// Mark the initial tunnel down
	for (int x = 30; x != 34; x++)
		for (int y = 4; y != 60; y++)
			objectMatrix[x][y] = 'T';

	// Initialize tunnelman
	m_tunnelman = new Tunnelman(this);

	// Initialize objects
	// Boulders
	B = min<unsigned int>(getLevel()/2 + 2, 9);

	for (int i = 0; i < B; i++)
	{
		int x = rand() % 61;
		int y = rand() % 61;

		if (!overlap(x, y))
		{
			objectV.push_back(new Boulder(x, y, this));
			digEarth(x, y);
			setObjectPosition(x, y, 'B');
		}
		else
		{
			i--;
		}
	}

	// Gold
	G = std::max<unsigned int>(5 - getLevel()/2, 2);
	for (int i = 0; i < G; i++)
	{
		int x = rand() % 61;
		int y = rand() % 61;

		if (!overlap(x, y))
		{
			objectV.push_back(new Gold(x, y, false, true, "permanent", this));
			setObjectPosition(x, y, 'G');
		}
		else
		{
			i--;
		}
	}

	// Oil
	L = std::min<unsigned int>(getLevel() + 2, 21);
	for (int i = 0; i < L; i++)
	{
		int x = rand() % 61;
		int y = rand() % 61;

		if (!overlap(x, y))
		{
			objectV.push_back(new Oil(x, y, this));
			setObjectPosition(x, y, 'L');
		}
		else
		{
			i--;
		}
	}

	// Prep for protesters initialization
	ticksSinceLastProtester = max<unsigned int>(25, 200 - getLevel());
	ticksForNextProtester = ticksSinceLastProtester;
	nProtesters = 0;

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::setObjectPosition(int x, int y, char object)
{
	for (int i = x; i != x + 4; i++)
		for (int j = y; j != y + 4; j++)
			objectMatrix[i][j] = object;
}

bool StudentWorld::digEarth(int x, int y)
{
	bool output = false;
	for (int i = 0; i < nEarth; i++)
	{
		if (earthArray[i] != nullptr)
		{
			if (earthArray[i]->getX() >= x && earthArray[i]->getX() <= x + 3 && earthArray[i]->getY() >= y && earthArray[i]->getY() <= y + 3)
			{
				delete earthArray[i];
				earthArray[i] = nullptr;
				output = true;
			}
		}
	}
	return output;
}

bool StudentWorld::overlap(int x, int y)
{
	vector<Object*>::iterator it;

	it = objectV.begin();

	while (it != objectV.end())
	{
		int dSquared = ((*it)->getX() - x) * ((*it)->getX() - x) + ((*it)->getY() - y) * ((*it)->getY() - y);
		if (sqrt(dSquared) <= 6)
			return true;
		else it++;
	}

	if (getObjectPosition(x, y) != 'E' || getObjectPosition(x + 3, y) != 'E' || getObjectPosition(x, y + 3) != 'E' || getObjectPosition(x + 3, y + 3) != 'E')
		return true;

	return false;
}

// MOVE and OTHERS
int StudentWorld::move()
{
	// Update Game Info
	setDisplayText();

	// Add new objects
	int hardcoreProb = min<unsigned int>(90, getLevel() * 10 + 30);
	int G = getLevel()*25 + 300;

	if (canAddProtester())
	{
		int n = rand() % 100 + 1;
		if (n <= hardcoreProb)
			objectV.push_back(new HardcoreProtester(this));
		else
			objectV.push_back(new RegularProtester(this));
	}
	
	int newGoodie = rand() % G + 1;

	if (newGoodie <= 1)
	{
		int whichGoodie = rand() % 5 + 1;
		if (whichGoodie <= 1)
			objectV.push_back(new Sonar(this));
		else if (whichGoodie > 1)
		{
			int x = rand() % 61;
			int y = rand() % 61;
			while (!canAddWater(x, y))
			{
				x = rand() % 61;
				y = rand() % 61;
			}

			objectV.push_back(new Water(x, y, this));
		}
	}

	// Check if player still alive
	if (m_tunnelman->isAlive())
	{
		m_tunnelman->doSomething();
	}

	// Check each object's status
	vector<Object*>::iterator it;
	it = objectV.begin();
	while (it != objectV.end())
	{
		if ((*it)->isAlive()) (*it)->doSomething();
		if (playerDied()) return GWSTATUS_PLAYER_DIED;
		if (finishedLevel()) return GWSTATUS_FINISHED_LEVEL;
		it++;
	}
	
	removeDeadGameObjects();

	if (playerDied())
	{
		return GWSTATUS_PLAYER_DIED;
	}

	if (finishedLevel())
	{
		GameController::getInstance().playSound(SOUND_FINISHED_LEVEL);
		return GWSTATUS_FINISHED_LEVEL;
	}

	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::setDisplayText()
{
	int level = getLevel();
	int lives = getLives();
	int health = m_tunnelman->getHP() * 10;
	int squirts = m_tunnelman->currentSquirts();
	int gold = m_tunnelman->currentGold();
	int barrelsLeft = B;
	int sonar = m_tunnelman->currentSonar();
	int score = getScore();

	// Next, create a string from your statistics, of the form:
	// Lvl: 52 Lives: 3 Hlth: 80% Wtr: 20 Gld: 3 Oil Left: 2 Sonar: 1 Scr: 321000
	ostringstream oss; // oss is a name of our choosing.
	oss.setf(ios::fixed);
	oss.precision(0);
	oss = outputFormatter(level, lives, health, squirts, gold, barrelsLeft, sonar, score);
	// Finally, update the display text at the top of the screen with your
	// newly created stats
	string s = oss.str();
	setGameStatText(s); // calls our provided GameWorld::setGameStatText
}

ostringstream StudentWorld::outputFormatter(int level, int lives, int health, int squirts, int gold, int barrelsLeft, int sonar, int score)
{
	ostringstream oss;
	oss << "Lvl: " << setw(2) << level
		<< " Lives: " << lives
		<< "  Hlth: " << setw(3) << health << "%"
		<< "  Wtr: " << setw(2) << squirts
		<< "  Gld: " << setw(2) << gold
		<< "  Oil Left: " << setw(2) << barrelsLeft
		<< "  Sonar: " << setw(2) << sonar
		<< "  Scr: ";
	oss.fill(0);
	oss << setw(6) << score;
	return oss;
}

bool StudentWorld::canAddProtester()
{
	if (ticksSinceLastProtester >= ticksForNextProtester && nProtesters < min<unsigned int>(15, 2 + getLevel() * 1.5))
	{
		ticksSinceLastProtester = 0;
		nProtesters++;
		return true;
	}
	else
	{
		ticksSinceLastProtester++;
		return false;
	}
}

bool StudentWorld::canAddWater(int currX, int currY)
{
	for (int i = 0; i != nEarth; i++)
	{
		if (earthArray[i] != nullptr)
		{
			if (earthArray[i]->getX() >= currX && earthArray[i]->getX() <= currX + 3 && earthArray[i]->getY() >= currY && earthArray[i]->getY() <= currY + 3)
				return false;
		}
	}

	return true;
}

void StudentWorld::removeDeadGameObjects()
{
	vector<Object*>::iterator it;
	it = objectV.begin();

	while (it != objectV.end())
	{
		if (!((*it)->isAlive()))
		{
			setObjectPosition((*it)->getX(), (*it)->getY(), 'T');
			if ((*it)->isProtester())
				nProtesters--;
			delete (*it);
			it = objectV.erase(it);
		}
		else it++;
	}
}

bool StudentWorld::finishedLevel()
{
	if (B == 0)
	{
		GameController::getInstance().playSound(SOUND_FINISHED_LEVEL);
		return true;
	}
	return false;
}

bool StudentWorld::playerDied()
{
	if (!m_tunnelman->isAlive())
	{
		decLives();
		return true;
	}

	return false;
}

// CLEAN UP and OTHERS
void StudentWorld::cleanUp()
{
	// Delete all earth
	for (int i = 0; i < nEarth; i++)
	{
		if (earthArray[i] != nullptr)
		{
			delete earthArray[i];
			earthArray[i] = nullptr;
		}
	}

	// Delete tunnelman
	delete m_tunnelman;

	// Delete other objects
	vector<Object*>::iterator it;
	it = objectV.begin();

	while (it != objectV.end())
	{
		delete (*it);
		it = objectV.erase(it);
	}

	for (int x = 0; x != 64; x++)
		for (int y = 0; y != 64; y++)
			objectMatrix[x][y] = 0;
}

