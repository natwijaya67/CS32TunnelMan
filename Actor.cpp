#include "Actor.h"
#include "StudentWorld.h"

using namespace std;

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

// Tunnelman related functions

void Tunnelman::doSomething()
{
	if (!isAlive()) return;

	if (getWorld()->digEarth(getX(), getY()))
	{
		GameController::getInstance().playSound(SOUND_DIG);
	}

	int key;
	if (getWorld()->getKey(key) == true)
	{
		switch (key)
		{
		case KEY_PRESS_LEFT:
			if (getDirection() != left)
			{
				setDirection(left);
			}

			else if (getX() >= 1 && !getWorld()->isBoulder(getX() - 1, getY(), left))
			{
				moveTo(getX() - 1, getY());
			}

			else if (getX() == 0)
			{
				moveTo(getX(), getY());
			}

			break;

		case KEY_PRESS_RIGHT:
			if (getDirection() != right)
			{
				setDirection(right);
			}

			else if (getX() <= 59 && !getWorld()->isBoulder(getX() + 1, getY(), right))
			{
				moveTo(getX() + 1, getY());
			}

			else if (getX() == 60)
			{
				moveTo(getX(), getY());
			}

			break;

		case KEY_PRESS_UP:
			if (getDirection() != up)
			{
				setDirection(up);
			}

			else if (getY() <= 59 && !getWorld()->isBoulder(getX(), getY() + 1, up))
			{
				moveTo(getX(), getY() + 1);
			}

			else if (getY() == 60)
			{
				moveTo(getX(), getY());
			}

			break;

		case KEY_PRESS_DOWN:
			if (getDirection() != down)
			{
				setDirection(down);
			}

			else if (getY() > 0 && !getWorld()->isBoulder(getX(), getY() - 1, down))
			{
				moveTo(getX(), getY() - 1);
			}

			else if (getY() == 0)
			{
				moveTo(getX(), getY());
			}

			break;

		case KEY_PRESS_SPACE:
			if (m_water > 0)
			{
				if (getDirection() == right && getX() < 60)
				{
					getWorld()->shootSquirt(getX() + 4, getY(), right);
					m_water--;
				}

				if (getDirection() == left && getX() > 0)
				{
					getWorld()->shootSquirt(getX() - 4, getY(), left);
					m_water--;
				}

				if (getDirection() == up && getY() < 60)
				{
					getWorld()->shootSquirt(getX(), getY() + 4, up);
					m_water--;
				}

				if (getDirection() == down && getX() > 0)
				{
					getWorld()->shootSquirt(getX(), getY() - 4, down);
					m_water--;
				}

				break;

		case KEY_PRESS_ESCAPE:
			setDead();
			break;

		case 'Z':
		case 'z':
			if (m_sonar > 0)
			{
				m_sonar--;
				getWorld()->showHidden(getX(), getY());
			}
			break;

		case KEY_PRESS_TAB:
			if (m_gold > 0)
			{
				getWorld()->dropGold(getX(), getY());
				m_gold--;
			}
			break;
			}

		}

	}
}

void Tunnelman::giveUp()
{
	setDead();
	GameController::getInstance().playSound(SOUND_PLAYER_GIVE_UP);
}

void Boulder::doSomething()
{
	if (!isAlive()) return;

	if (m_state == "stable")
	{
		if (!(getWorld()->isEarth(getX(), getY() - 1, down)))
		{
			m_state = "waiting";
			m_waitTicks = 0;
		}
	}

	else if (m_state == "waiting" && m_waitTicks < 30) m_waitTicks++;

	else if (m_state == "waiting" && m_waitTicks >= 30)
	{
		m_state = "falling";
		GameController::getInstance().playSound(SOUND_FALLING_ROCK);
	}

	else if (m_state == "falling")
	{
		if (getY() >= 1 && !(getWorld()->isEarth(getX(), getY() - 1, down)) && !(getWorld()->isBoulder(getX(), getY() - 1, down)))
		{
			getWorld()->setObjectPosition(getX(), getY(), 'T');
			moveTo(getX(), getY() - 1);
			getWorld()->annoyTunnelman(getX(), getY());
			getWorld()->annoyProtester(getX(), getY(), 100);
			getWorld()->setObjectPosition(getX(), getY(), 'B');
		}
		else
		{
			setDead();
		}
	}
}

void Squirt::doSomething()
{
	if (getWorld()->annoyProtester(getX(), getY(), 2))
	{
		setDead();
	}

	if (travelD == 0)
	{	
		setDead();
	}

	else if (getDirection() == right)
	{
		if (getWorld()->isEarth(getX() + 1, getY(), right) || getWorld()->isBoulder(getX() + 1, getY(), right) || getX() >= 60)
			setDead();
		else
		{
			moveTo(getX() + 1, getY());
			travelD--;
		}
	}

	else if (getDirection() == left)
	{
		if (getWorld()->isEarth(getX() - 1, getY(), left) || getWorld()->isBoulder(getX() - 1, getY(), left) || getX() <= 0)
			setDead();
		else
		{
			moveTo(getX() - 1, getY());
			travelD--;
		}
	}

	else if (getDirection() == up)
	{
		if (getWorld()->isEarth(getX(), getY() + 1, up) || getWorld()->isBoulder(getX(), getY() + 1, up) || getY() >= 60)
			setDead();
		else
		{
			moveTo(getX(), getY() + 1);
			travelD--;
		}
	}

	else if (getDirection() == down)
	{
		if (getWorld()->isEarth(getX(), getY() - 1, down) || getWorld()->isBoulder(getX(), getY() - 1, down) || getY() <= 0)
			setDead();
		else
		{
			moveTo(getX(), getY() - 1);
			travelD--;
		}
	}
}

// Goodies related functions

Goodies::Goodies(int imageID, int initX, int initY, Direction initDirection, double size, unsigned int depth, StudentWorld* world)
	:Object(imageID, initX, initY, initDirection, size, depth, world)
{
	m_restTicksLeft = max<unsigned int>(100, 300 - 10 * (getWorld()->getLevel()));
}

void Oil::doSomething()
{
	if (!isAlive()) return;

	if (!is_visible() && getWorld()->nearTunnelman(getX(), getY(), 4))
	{
		setVisible(true);
		return;
	}
	else if (getWorld()->nearTunnelman(getX(), getY(), 3))
	{
		setDead();
		GameController::getInstance().playSound(SOUND_FOUND_OIL);
		getWorld()->increaseScore(1000);
		getWorld()->setObjectPosition(getX(), getY(), 'T');
		getWorld()->decreaseBarrel();
	}
}

void Gold::doSomething()
{
	if (!isAlive())
		return;

	if (!isVisible() && getWorld()->nearTunnelman(getX(), getY(), 4))
	{
		setVisible(true);
		return;
	}
	else if (can_pickup() && getWorld()->nearTunnelman(getX(), getY(), 3))
	{
		setDead();
		GameController::getInstance().playSound(SOUND_GOT_GOODIE);
		getWorld()->increaseScore(10);
		getWorld()->getTunnelman()->addGold();
	}
	else if (!can_pickup())
	{
		Object* nearestProtester = getWorld()->findProtester(getX(), getY());
		if (nearestProtester == nullptr)
		{
			if (getRestTicks() <= 0)
			{
				setDead();
				return;
			}
			else
			{
				reduceRestTicks();
			}
		}
		else
		{
			setDead();
			nearestProtester->getBribed();
		}
	}
}

void Sonar::doSomething()
{
	if (!isAlive()) return;

	if (getWorld()->nearTunnelman(getX(), getY(), 3))
	{
		setDead();
		GameController::getInstance().playSound(SOUND_GOT_GOODIE);
		getWorld()->getTunnelman()->addKit();
		getWorld()->increaseScore(75);
	}
	else if (getRestTicks() == 0)
		setDead();
	else reduceRestTicks();
}

void Water::doSomething()
{
	if (!isAlive()) return;

	if (getWorld()->nearTunnelman(getX(), getY(), 3))
	{
		setDead();
		GameController::getInstance().playSound(SOUND_GOT_GOODIE);
		getWorld()->getTunnelman()->addSquirts();
		getWorld()->increaseScore(100);
	}
	else if (getRestTicks() == 0)
		setDead();
	else reduceRestTicks();
}

// Protester-related functions

Protester::Protester(int imageID, StudentWorld* world)
	: Person(imageID, 60, 60, left, 1.0, 0, world)
{
	m_numSquaresToMoveInCurrDir = rand() % 53 + 8;
	m_ticksToWaitBetweenMoves = max<unsigned int>(0, 3 - (getWorld()->getLevel())/4);
	m_restTicksLeft = m_ticksToWaitBetweenMoves;
	m_ticksSinceShout = 15;
	m_ticksSinceTurn = 0;
	m_state = "rest";
}

bool Protester::findPath(int endX, int endY, int initX, int initY, Direction& nextStep, int& steps)
{
	int stepsMatrix[64][64];
	bool output = false;
	queue<Point> exitPath;
	int currSteps;
	int minSteps;

	for (int i = 0; i != 64; i++)
		for (int j = 0; j != 64; j++)
			stepsMatrix[i][j] = 9999;

	exitPath.push(Point(endX, endY));
	stepsMatrix[endX][endY] = 0;

	while (!exitPath.empty())
	{
		int currX = exitPath.front().m_x;
		int currY = exitPath.front().m_y;
		exitPath.pop();

		if (currX == initX && currY == initY)
		{
			output = true;
			break;
		}

		currSteps = stepsMatrix[currX][currY];
		currSteps++;

		if ((currX > 0) && !(getWorld()->isEarth(currX - 1, currY, left)) && !(getWorld()->isEarth(currX - 1, currY, left)) && (stepsMatrix[currX - 1][currY] == 9999)) //check left
		{
			stepsMatrix[currX - 1][currY] = currSteps;
			exitPath.push(Point(currX - 1, currY));
		}

		if ((currX < 60) && !(getWorld()->isEarth(currX + 1, currY, right)) && !(getWorld()->isEarth(currX + 1, currY, right)) && (stepsMatrix[currX + 1][currY] == 9999)) //check right
		{
			stepsMatrix[currX + 1][currY] = currSteps;
			exitPath.push(Point(currX - 1, currY));
		}

		if ((currY > 1) && !(getWorld()->isEarth(currX, currY - 1, down)) && !(getWorld()->isEarth(currX, currY - 1, down)) && (stepsMatrix[currX][currY - 1] == 9999)) //check down
		{
			stepsMatrix[currX][currY - 1] = currSteps;
			exitPath.push(Point(currX, currY - 1));
		}

		if ((currX < 60) && !(getWorld()->isEarth(currX, currY + 1, left)) && !(getWorld()->isEarth(currX, currY + 1, left)) && (stepsMatrix[currX][currY + 1] == 9999)) //check left
		{
			stepsMatrix[currX][currY + 1] = currSteps;
			exitPath.push(Point(currX, currY + 1));
		}
	}

	if (initX == 0 && initY == 0)
	{
		minSteps = min(stepsMatrix[initX + 1][initY], stepsMatrix[initX][initY + 1]);
		if (minSteps == stepsMatrix[initX + 1][initY])
			nextStep = right;
		else
			nextStep = up;
	}
	else if (initX == 0)
	{
		minSteps = min(stepsMatrix[initX + 1][initY], min(stepsMatrix[initX][initY + 1], stepsMatrix[initX][initY - 1]));
		if (minSteps == stepsMatrix[initX + 1][initY])
			nextStep = right;
		else if (minSteps == stepsMatrix[initX][initY + 1])
			nextStep = up;
		else
			nextStep = down;
	}

	else if (initY == 0)
	{
		minSteps = min(stepsMatrix[initX][initY + 1], min(stepsMatrix[initX - 1][initY], stepsMatrix[initX + 1][initY]));
		if (minSteps == stepsMatrix[initX + 1][initY])
			nextStep = right;
		else if (minSteps == stepsMatrix[initX][initY + 1])
			nextStep = up;
		else
			nextStep = left;
	}
	else
	{
		minSteps = min(min(stepsMatrix[initX][initY + 1], stepsMatrix[initX][initY - 1]), min(stepsMatrix[initX - 1][initY], stepsMatrix[initX + 1][initY]));
		if (minSteps == stepsMatrix[initX + 1][initY])
			nextStep = right;
		else if (minSteps == stepsMatrix[initX][initY + 1])
			nextStep = up;
		else if (minSteps == stepsMatrix[initX - 1][initY])
			nextStep = left;
		else
			nextStep = down;

	}

	steps = minSteps;
	return output;
}

void Protester::loseHealth(int damage)
{
	Person::loseHealth(damage);

	if (getHP() > 0)
	{
		GameController::getInstance().playSound(SOUND_PROTESTER_ANNOYED);
		getStunned();
	}

	if (getHP() <= 0)
	{
		if (damage == 2)
		{
			if (isHardcore())
			{
				getWorld()->increaseScore(250);
			}
			else
			{
				getWorld()->increaseScore(100);
			}
		}
		else if (damage == 100)
		{
			getWorld()->increaseScore(500);
		}
	}
}

void Protester::getStunned()
{
	m_restTicksLeft = max<unsigned int>(50, 100 - (getWorld()->getLevel())*10);
	m_state = "rest";
}

void Protester::giveUp()
{
	setLeaveState();
	GameController::getInstance().playSound(SOUND_PROTESTER_GIVE_UP);
}

void Protester::shout()
{
	GameController::getInstance().playSound(SOUND_PROTESTER_YELL);
	getWorld()->getTunnelman()->loseHealth(2);
}

void RegularProtester::doSomething()
{
	Direction nextDir = getDirection();
	if (!isAlive()) return;

	if (getRestTicksLeft() > 0)
	{
		doNothing();
		return;
	}

	if (getState() == "leave-the-oil-field")
	{
		if (getX() == 60 && getY() == 60)
		{
			setDead();
			return;
		}

		int steps;
		if (findPath(60, 60, getX(), getY(), nextDir, steps))
		{
			if (nextDir == up)
			{
				setDirection(up);
				moveTo(getX(), getY() + 1);
			}
			else if (nextDir == down)
			{
				setDirection(down);
				moveTo(getX(), getY() - 1);
			}
			else if (nextDir == left)
			{
				setDirection(left);
				moveTo(getX() - 1, getY());
			}
			else if (nextDir == right)
			{
				setDirection(right);
				moveTo(getX() + 1, getY());
			}
		}

		endTurn();
		return;
	}

	if (getWorld()->canShout(getX(), getY(), getDirection()))
	{
		if (getTicksSinceShout() > 15)
		{
			shout();
			resetShoutTicks();
			endTurn();
			return;
		}
		else if (getTicksSinceShout() <= 15)
		{
			endTurn();
			return;
		}
	}

	if (getWorld()->facingTunnelman(getX(), getY(), nextDir))
	{
		setDirection(nextDir);
		if (nextDir == up)
			moveTo(getX(), getY() + 1);
		else if (nextDir == down)
			moveTo(getX(), getY() - 1);
		else if (nextDir == left)
			moveTo(getX() - 1, getY());
		else if (nextDir == right)
			moveTo(getX() + 1, getY());

		resetNumSquaresToMoveInCurrDir();
		endTurn();
		return;
	}

	if (!(getWorld()->facingTunnelman(getX(), getY(), nextDir)))
	{
		decreaseNumSquaresToMoveInCurrDir();
	}

	// Step 6
	if (getNumSquaresToMoveInCurrDir() <= 0)
	{
		nextDir = getWorld()->newDirRandom(getX(), getY());
		setDirection(nextDir);
		setNumSquaresToMoveInCurrDir();
	}
	// Step 7
	else if (getTicksSinceTurn() > 200)
	{
		if (getDirection() == right || getDirection() == left)
		{
			if (!(getWorld()->isEarth(getX(), getY() + 1, up)) && !(getWorld()->isBoulder(getX(), getY() + 1, up)) && getY() < 60)
			{
				setDirection(up);
				setNumSquaresToMoveInCurrDir();
				resetTurnTicks();
			}
			else if (!(getWorld()->isEarth(getX(), getY() - 1, down)) && !(getWorld()->isBoulder(getX(), getY() - 1, down)) && getY() > 0)
			{
				setDirection(down);
				setNumSquaresToMoveInCurrDir();
				resetTurnTicks();
			}
		}
		else if (getDirection() == up || getDirection() == down)
		{
			if (!(getWorld()->isEarth(getX() - 1, getY(), left)) && !(getWorld()->isBoulder(getX() - 1, getY(), left)) && getX() > 0)
			{
				setDirection(left);
				setNumSquaresToMoveInCurrDir();
				resetTurnTicks();
			}
			else if (!(getWorld()->isEarth(getX() + 1, getY(), right)) && !(getWorld()->isBoulder(getX() + 1, getY(), right)) && getX() < 60)
			{
				setDirection(right);
				setNumSquaresToMoveInCurrDir();
				resetTurnTicks();
			}
		}
	}

	// Step 8 and 9
	if (getDirection() == left)
	{
		if (!(getWorld()->isEarth(getX() - 1, getY(), left)) && !(getWorld()->isBoulder(getX() - 1, getY(), left)) && getX() > 0)
			moveTo(getX() - 1, getY());
		else
			resetNumSquaresToMoveInCurrDir();
	}
	else if (getDirection() == right)
	{
		if (!(getWorld()->isEarth(getX() + 1, getY(), right)) && !(getWorld()->isBoulder(getX() + 1, getY(), right)) && getX() < 60)
			moveTo(getX() + 1, getY());
		else
			resetNumSquaresToMoveInCurrDir();
	}

	else if (getDirection() == up)
	{
		if (!(getWorld()->isEarth(getX(), getY() + 1, up)) && !(getWorld()->isBoulder(getX(), getY() + 1, up)) && getY() < 60)
			moveTo(getX(), getY() + 1);
		else
			resetNumSquaresToMoveInCurrDir();
	}
	else if (getDirection() == down)
	{
		if (!(getWorld()->isEarth(getX(), getY() - 1, down)) && !(getWorld()->isBoulder(getX(), getY() - 1, down)) && getY() > 0)
			moveTo(getX(), getY() - 1);
		else
			resetNumSquaresToMoveInCurrDir();
	}

	endTurn();
	return;
}

void RegularProtester::getBribed()
{
	GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
	getWorld()->increaseScore(25);
	setLeaveState();
}

void HardcoreProtester::doSomething()
{
	Direction nextDir = getDirection();
	if (!isAlive()) return;

	if (getRestTicksLeft() > 0)
	{
		doNothing();
		return;
	}

	if (getState() == "leave-the-oil-field")
	{
		if (getX() == 60 && getY() == 60)
			setDead();

		int steps;
		if (findPath(60, 60, getX(), getY(), nextDir, steps))
		{
			if (nextDir == up)
			{
				setDirection(up);
				moveTo(getX(), getY() + 1);
			}
			else if (nextDir == down)
			{
				setDirection(down);
				moveTo(getX(), getY() - 1);
			}
			else if (nextDir == left)
			{
				setDirection(left);
				moveTo(getX() - 1, getY());
			}
			else if (nextDir == right)
			{
				setDirection(right);
				moveTo(getX() + 1, getY());
			}
		}

		endTurn();
		return;
	}

	if (getWorld()->canShout(getX(), getY(), getDirection()))
	{
		if (getTicksSinceShout() > 15)
		{
			shout();
			resetShoutTicks();
			endTurn();
			return;
		}

		else if (getTicksSinceShout() <= 15)
		{
			endTurn();
			return;
		}
	}

	if (getWorld()->nearTunnelman(getX(), getY(), 4))
	{
		int M = 16 + getWorld()->getLevel() * 2;
		int steps;
		findPath(getWorld()->getTunnelman()->getX(), getWorld()->getTunnelman()->getY(), getX(), getY(), nextDir, steps);

		if (steps <= M)
		{
			setDirection(nextDir);
			if (nextDir == up)
				moveTo(getX(), getY() + 1);
			else if (nextDir == down)
				moveTo(getX(), getY() - 1);
			else if (nextDir == left)
				moveTo(getX() - 1, getY());
			else if (nextDir == right)
				moveTo(getX() + 1, getY());
		}

		endTurn();
		return;
	}

	if (getWorld()->facingTunnelman(getX(), getY(), nextDir))
	{
		setDirection(nextDir);
		switch (nextDir)
		{
		case up:
			moveTo(getX(), getY() + 1);
		case down:
			moveTo(getX(), getY() - 1);
		case left:
			moveTo(getX() - 1, getY());
		case right:
			moveTo(getX() + 1, getY());
		}


		resetNumSquaresToMoveInCurrDir();
		endTurn();
		return;
	}

	if (!(getWorld()->facingTunnelman(getX(), getY(), nextDir)))
	{
		decreaseNumSquaresToMoveInCurrDir();
	}

	if (getNumSquaresToMoveInCurrDir() <= 0)
	{
		nextDir = getWorld()->newDirRandom(getX(), getY());
		setDirection(nextDir);
		setNumSquaresToMoveInCurrDir();
	}

	else if (getTicksSinceTurn() > 200)
	{
		if (getDirection() == right || getDirection() == left)
		{
			if (!(getWorld()->isEarth(getX(), getY() + 1, up)) && !(getWorld()->isBoulder(getX(), getY() + 1, up)) && getY() < 60)
			{
				setDirection(up);
				setNumSquaresToMoveInCurrDir();
				resetTurnTicks();
			}

			else if (!(getWorld()->isEarth(getX(), getY() - 1, down)) && !(getWorld()->isBoulder(getX(), getY() - 1, down)) && getY() > 0)
			{
				setDirection(down);
				setNumSquaresToMoveInCurrDir();
				resetTurnTicks();
			}
		}

		else if (getDirection() == up || getDirection() == down)
		{
			if (!(getWorld()->isEarth(getX() - 1, getY(), left)) && !(getWorld()->isBoulder(getX() - 1, getY(), left)) && getX() > 0)
			{
				setDirection(left);
				setNumSquaresToMoveInCurrDir();
				resetTurnTicks();
			}

			else if (!(getWorld()->isEarth(getX() + 1, getY(), right)) && !(getWorld()->isBoulder(getX() + 1, getY(), right)) && getX() < 60)
			{
				setDirection(right);
				setNumSquaresToMoveInCurrDir();
				resetTurnTicks();
			}
		}
	}

	if (getDirection() == left)
	{
		if (!(getWorld()->isEarth(getX() - 1, getY(), left)) && !(getWorld()->isBoulder(getX() - 1, getY(), left)) && getX() > 0)
			moveTo(getX() - 1, getY());
		else
			resetNumSquaresToMoveInCurrDir();
	}
	else if (getDirection() == right)
	{
		if (!(getWorld()->isEarth(getX() + 1, getY(), right)) && !(getWorld()->isBoulder(getX() + 1, getY(), right)) && getX() < 60)
			moveTo(getX() + 1, getY());
		else
			resetNumSquaresToMoveInCurrDir();
	}

	else if (getDirection() == up)
	{
		if (!(getWorld()->isEarth(getX(), getY() + 1, up)) && !(getWorld()->isBoulder(getX(), getY() + 1, up)) && getY() < 60)
			moveTo(getX(), getY() + 1);
		else
			resetNumSquaresToMoveInCurrDir();
	}
	else if (getDirection() == down)
	{
		if (!(getWorld()->isEarth(getX(), getY() - 1, down)) && !(getWorld()->isBoulder(getX(), getY() - 1, down)) && getY() > 0)
			moveTo(getX(), getY() - 1);
		else
			resetNumSquaresToMoveInCurrDir();
	}

	endTurn();
	return;
}

void HardcoreProtester::getBribed()
{
	GameController::getInstance().playSound(SOUND_PROTESTER_FOUND_GOLD);
	getWorld()->increaseScore(50);
	getStunned();
}
		