#include "EnemyMgr.h"
#include "BulletMgr.h"

#include "Mosque.h"
#include "Kessie.h"
#include "ArabMelee.h"
#include "ArabBurserker.h"

#include "ProcPlayer.h"
#include "ProcField.h"

/////////////////////////////////
Mosque* mosque;
Kessie* kessie;
/////////////////////////////////
ProcEnemy*** _enemies;
ProcEnemy** enemies;
int enemyCount;
void loadProcEnemy()
{
	_enemies = new ProcEnemy * * [EnemyIndexMax];
	for (int i = 0; i < EnemyIndexMax; i++)
	{
		_enemies[i] = new ProcEnemy * [enemyMax];
		if (i == 0)
		{
			for (int j = 0; j < enemyMax; j++)
				_enemies[i][j] = new ArabMelee(i);
		}
		else if (i == 1)
		{
			for (int j = 0; j < enemyMax; j++)
				_enemies[i][j] = new ArabBurserker(i);
		}
	}
	enemies = new ProcEnemy * [EnemyIndexMax * enemyMax];
	enemyCount = 0;

	//addProcEnemy(ArMelee, iPointMake(200, 50));
	addProcEnemy(ArMelee, iPointMake(200, 50), iPointMake(-1,0), AI::enemyAI1);
	addProcEnemy(ArBurserker, iPointMake(230, 50), iPointMake(-1, 0), AI::enemyAIBurserker);
#if 1
	mosque = new Mosque();
	mosque->initObj();

	kessie = new Kessie();
	kessie->init();
#endif

}
void freeProcEnemy()
{
	for (int i = 0; i < EnemyIndexMax; i++)
	{
		for (int j = 0; j < enemyMax; j++)
		{
			_enemies[i][j]->freeObj();
			delete _enemies[i][j];
		}
		delete _enemies[i];
	}
	delete _enemies;
	delete enemies;

#if 1
	delete mosque;
#endif
}
void drawProcEnemy(float dt, iPoint off)
{
#if 0
	mosque->updateObj(dt);
	mosque->drawObj(dt, off);
#endif
#if 1
	kessie->update(dt);
	kessie->draw(dt, off);
#endif
	for (int i = 0; i < enemyCount; i++)
	{
		ProcEnemy* e = enemies[i];

		if (e->isActive)
		{
			//if (e->getState() != (EnemyBehave)(eDeadL + (e->state % 2)))
			e->updateObj(dt);
			e->drawObj(dt, off);
		}

		if (e->isActive == false)
		{
			enemyCount--;
			enemies[i] = enemies[enemyCount];
			i--;
		}

	}
}
void addProcEnemy(int idx, iPoint p)
{
	for (int i = 0; i < enemyMax; i++)
	{
		ProcEnemy* e = _enemies[idx][i];
		if (e->isActive == false)
		{
			e->isActive = true;
			e->p = p;
			e->idx = (EnemyIndex)idx;
			e->initObj();
			enemies[enemyCount] = e;
			enemyCount++;
			return;
		}
	}
}

void addProcEnemy(int idx, iPoint p, iPoint v, EnemyAI ai)
{
	for (int i = 0; i < enemyMax; i++)
	{
		ProcEnemy* e = _enemies[idx][i];
		if (e->isActive == false)
		{
			e->isActive = true;
			e->p = p;
			e->idx = (EnemyIndex)idx;

			//#issue
			e->initObj();
			enemies[enemyCount] = e;
			enemyCount++;
			return;
		}
	}
}

void AI::enemyAI0(ProcEnemy* e, float dt)		//Check Player
{
	int len = iPointLength(player->p - e->p);

	if (len < e->sight)
		e->tp = player->p;
	else
		e->tp = { -1,-1 };

	if (e->tp != iPointMake(-1, -1))
	{
		e->v = e->tp - e->p;
		e->v /= iPointLength(e->v);

		if (e->v.x > 0)
		{
			e->tp.x -= e->attkRange;
			e->setState(WalkEnemyR);
		}
		else if (e->v.x < 0)
		{
			e->tp.x += e->attkRange;
			e->setState(WalkEnemyL);
		}
		else
		{
			e->setState((EnemyBehave)(IdleEnemyL + e->state % 2));
		}
	}
}

void AI::enemyAI1(ProcEnemy* e, float dt)
{
	if (movePoint(e->p, e->p, e->tp, e->moveSpeed))
	{
	}
	//e->p += e->v * e->moveSpeed * dt;
}

void AI::enemyAIBurserker(ProcEnemy* e, float dt)		//Spawn From Kessie
{
	int len = iPointLength(player->p - e->p);

	if (len < e->sight)
		e->tp.x = bg->off.x;
	else
		e->tp = { -1,-1 };

	if (e->tp != iPointMake(-1, -1))
	{
		e->v = e->tp - e->p;
		e->v /= iPointLength(e->v);

		if (e->v.x > 0)
		{
			e->tp.x -= e->attkRange;
			e->setState(WalkEnemyR);
		}
		else if (e->v.x < 0)
		{
			e->tp.x += e->attkRange;
			e->setState(WalkEnemyL);
		}
		else
		{
			e->setState((EnemyBehave)(IdleEnemyL + e->state % 2));
		}
	}
}