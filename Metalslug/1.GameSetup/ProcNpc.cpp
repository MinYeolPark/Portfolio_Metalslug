#include "ProcNpc.h"
#include "ProcNpc.h"

#include "Ichimondi.h"

#include "ProcItem.h"
#include "ProcPlayer.h"
ProcNpc*** _npcs;
ProcNpc** npcs;
int npcNum;
void loadProcNpcs()
{
	_npcs = new ProcNpc * * [NpcIndexMax];
	for (int i = 0; i < NpcIndexMax; i++)
	{
		_npcs[i] = new ProcNpc * [npcMax];
		if (i == NpcIchimondi)
			for (int j = 0; j < npcMax; j++)
				_npcs[i][j] = new Ichimondi(i);
	}
	npcs = new ProcNpc * [NpcIndexMax * npcMax];
	npcNum = 0;

	//AddProcNpc
	//addProcNPC(NpcIchimondi, iPointMake(1000, 100),ItemHeavyMachineGun);
	addProcNPC(NpcIchimondi, iPointMake(2000, 100), ItemHeavyMachineGun);
	//addProcNPC(NpcIchimondi, iPointMake(3400, 100), ItemHeavyMachineGun);
}

void freeProcNpcs()
{
	for (int i = 0; i < NpcIndexMax; i++)
	{
		delete _npcs[i];
		delete npcs[i];
	}
	delete _npcs;
	delete npcs;
}

void drawProcNpcs(float dt, iPoint off)
{
	for (int i = 0; i < npcNum; i++)
	{
		ProcNpc* n = npcs[i];
		n->update(dt);
		if (n->draw(dt, off))
		{
			npcNum--;
			npcs[i] = npcs[npcNum];
			i--;
		}
	}
}

void addProcNPC(int index, iPoint p)
{
	for (int i = 0; i < npcMax; i++)
	{
		ProcNpc* n = _npcs[index][i];
		if (n->isActive == false)
		{
			int r = rand() % ItemIndexMax;
			n->init(index, p, r);
			npcs[npcNum] = n;
			npcNum++;
			return;
		}
	}
}

void addProcNPC(int index, iPoint p, int itemIndex)
{
	for (int i = 0; i < npcMax; i++)
	{
		ProcNpc* n = _npcs[index][i];
		if (n->isActive == false)
		{
			n->init(index, p, itemIndex);
			npcs[npcNum] = n;
			npcNum++;
			return;
		}
	}
}

ProcNpc::ProcNpc(int index)
{
	
}

ProcNpc::~ProcNpc()
{

}

void ProcNpc::getDamage(float damage)
{
	if (state == (NpcBehave)(IdleNpcL + state % 2))
		state = (NpcBehave)(ReleaseNpcL + state % 2);
}

void ProcNpc::init(int idex, iPoint p, int itemIndex)
{
	isAppear = false;
	isActive = true;
	this->itemIndex = itemIndex;
	this->p = p;
	this->index = (NpcIndex)index;
}

void ProcNpc::fixedUpdate(float dt)
{
	int maxY = *(map->maxY + (int)p.x);
	if (p.y >= maxY)
	{
		up = 0;
		down = 0;
		fall = false;

		p.y = maxY;
	}
	else
		fall = true;

	if (fall)
	{
		if (p.y < maxY)
		{
			down += jumpDecrease * dt;
			p = (iPointMake(p.x, p.y += down));
		}
	}
}

