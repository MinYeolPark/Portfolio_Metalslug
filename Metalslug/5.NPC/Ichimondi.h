#pragma once
#include "ProcNpc.h"

class Ichimondi:
	public ProcNpc
{
public:
	Ichimondi(int idx);
	virtual ~Ichimondi();
public:
	iImage** imgs;
	iImage* imgCurr;
public:
	void dead();
	void spawnItem();

	virtual void init(int index, iPoint p, int itemIndex) override;
	virtual void update(float dt) override;
	virtual void fixedUpdate(float dt) override;
	virtual bool draw(float dt, iPoint off) override;
};

