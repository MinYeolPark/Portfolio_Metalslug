#pragma once
#include "ProcObject.h"

enum EnemyIndex
{
	IdxArMelee = 0,
	IdxArMeleeKessie,
	IdxMosque,
	//IdxArBurserker,
	//IdxArCamel,
	//IdxTruck,
	//IdxKessie,
	//IdxAbul,

	EnemyIndexMax,
};

class Collider;
class ProcEnemy;
typedef void(*EnemyAI)(ProcEnemy* e, float dt);
class ProcEnemy: 
	public ProcObject
{
public:
	ProcEnemy(int index);
	virtual ~ProcEnemy();
public:
	int state;

	float up;
	float down;
	bool fall;
	EnemyAI ai;

	iPoint firePoint;
	//stats
	int dmg;
	float sight;
	float moveSpeed;
	float attkRange;
	float attkRate, _attkRate;
	float aiDt, _aiDt;

	bool isAppear;
public:	
	virtual void getDamage(float damage);
	virtual void setState(int newState) = 0;
public:
	virtual void init(iPoint p) = 0;
	virtual void fixedUpdate(float dt);
	virtual void free() = 0;		//freeImage
};

