#include "Abul.h"

#include "ImgMgr.h"
#include "AnimationMgr.h"
ImageInfo imgAbulInfo[];
static iImage** _imgAbul = NULL;
Abul::Abul(int index) : ProcEnemy(index)
{
	this->index = index;
	state = IdleAbulL;

	hp = 0;
	dmg = 0;
	sight = 0;
	moveSpeed = 200.f;
	attkRange = 0;
	attkRate = 0.f;	_attkRate = 0.f;
	aiDt = 0.f;	_aiDt = 0.f;

    imgs = NULL;
    imgCurr = NULL;

	up = 0.f;
	down = 0.f;
	fall = false;

	if (_imgAbul == NULL)
		_imgAbul = createImgReverse(imgAbulInfo, AbulBehaveMax, this);

	imgs = new iImage * [AbulBehaveMax];
	memset(imgs, 0x00, sizeof(iImage*) * AbulBehaveMax);
	for (int i = 0; i < AbulBehaveMax; i++)
		imgs[i] = _imgAbul[i]->clone();
	imgCurr = imgs[index];
}

Abul::~Abul()
{
}

void Abul::getDamage(float damage)
{
	
}

void Abul::init(iPoint p)
{
	isActive = true;
	isAppear = false;
	this->p = p;
	tp = iPointMake(player->p.x + devSize.width/2, player->p.y);

	state = IdleAbulL;
}

void Abul::update(float dt)
{
	if (isDead)
		return;

	if (!isAppear)
	{
		if (tp == p)
		{
			isAppear = true;
			state = (SignalAbulL + state % 2);			
		}
	}
	else
	{
		isActive = containPoint(p,
			iRectMake(-map->off.x - 40, -map->off.y - 40,
				devSize.width + 80, devSize.height + 80));
	}

	int len = iPointLength(tp - p);
	iPoint v = tp - p;
	v /= iPointLength(v);
	this->v = v;

	if (p != tp)
	{
		if (p.x < tp.x)
		{
			p.x += v.x;
			if (p.x > tp.x)
				p.x = tp.x;
		}
		else if (p.x > tp.x)
		{
			p.x += v.x;
			if (p.x < tp.x)
				p.x = tp.x;
		}

		if (p.y < tp.y)
		{
			p.y += v.y;
			if (p.y > tp.y)
				p.y = tp.y;
		}
		else if (p.y > tp.y)
		{
			p.y += v.y;
			if (p.y < tp.y)
				p.y = tp.y;
		}

		if (p == tp)
			v = iPointZero;
	}

	//Animation
	if (v != iPointZero)
	{
		if (v.x > 0)
			state = WalkAbulR;
		else if (v.x < 0)
			state = WalkAbulL;
	}

	fixedUpdate(dt);
}

void Abul::fixedUpdate(float dt)
{
	ProcEnemy::fixedUpdate(dt);
}

bool Abul::draw(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[state];
	imgCurr->paint(dt, p + off);

	setRGBA(1, 1, 1, 1);

    return !isActive;
}

void Abul::free()
{
}



ImageInfo imgAbulInfo[] =
{
	{
		"assets/Abul/Abul_Idle_%02d.png",
		4, 1.0f, { -70 / 2, 0},
		0.1f,
		0,			//repeat
		{255, 0, 0, 255},
		NULL
	},
	{
		"assets/Abul/Abul_Walk_%02d.png",
		6, 1.0f, { -70 / 2, 0},
		0.1f,
		0,			//repeat
		{255, 0, 0, 255},
		NULL
	},
	{
		"assets/Abul/Abul_Signal_%02d.png",
		9, 1.0f, { -70 / 2, 0},
		0.1f,
		0,			//repeat
		{255, 0, 0, 255},
		NULL
	},
	{
		"assets/Abul/Abul_Surrender_%02d.png",
		17, 1.0f, { -70 / 2, 0},
		0.1f,
		0,			//repeat
		{255, 0, 0, 255},
		NULL
	}
};