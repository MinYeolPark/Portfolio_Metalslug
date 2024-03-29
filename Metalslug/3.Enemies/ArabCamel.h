#pragma once
#include "ProcEnemy.h"

enum ArabCamelBehave
{
	ArabCamelIdleL,
	ArabCamelIdleR,
	ArabCamelAppearL,
	ArabCamelAppearR,
	ArabCamelFireL,
	ArabCamelFireR,
	ArabCamelDownL,
	ArabCamelDownR,
	ArabCamelDownIdleL,
	ArabCamelDownIdleR,
	ArabCamelDownFireL,
	ArabCamelDownFireR,
	ArabCamelDeadL,
	ArabCamelDeadR,

	ArabCamelBehaveMax,
};
enum CamelBehave
{
	CamelRunL,
	CamelRunR,
	CamelDownL,
	CamelDownR,
	CamelDownIdleL,
	CamelDownIdleR,
	CamelDeadL,
	CamelDeadR,

	CamelBehaveMax,
};
class ArabCamel:
	public ProcEnemy
{
	public:
		ArabCamel(int index);
		~ArabCamel();

	public:
		iImage** camelImgs;
		iImage* camelImgCurr;
		iImage** arabImgs;
		iImage* arabImgCurr;

		iPoint arabPos;

		CamelBehave camelState;
		ArabCamelBehave arabState;
	public:
		virtual void getDamage(float damage);
	public:
		virtual void init(iPoint p);
		virtual void update(float dt);
		virtual void fixedUpdate(float dt);
		virtual bool draw(float dt, iPoint off);
		virtual void free();
};

