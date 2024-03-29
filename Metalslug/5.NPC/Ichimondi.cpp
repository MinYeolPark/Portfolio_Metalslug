#include "Ichimondi.h"

#include "ImgMgr.h"
#include "AnimationMgr.h"
#include "UIMgr.h"

#include "ProcItem.h"
static iImage** _imgIchi = NULL;
ImageInfo infoIchi[];
Ichimondi::Ichimondi(int index): ProcNpc(index)
{		
	score = 100;
	index = NpcIchimondi;
	state = IdleNpcL;
	complete = false;
	escape = false;
	itemIndex = 0;
	///////////////////////////////////
	imgs = NULL;
	imgCurr = NULL;

	moveSpeed = 50.f;
	up = 0.f;
	down = 0.f;
	fall = false;

	if(_imgIchi == NULL)
		_imgIchi = createImgReverse(infoIchi, NpcBehaveMax, this);

	imgs = new iImage * [NpcBehaveMax];
	memset(imgs, 0x00, sizeof(iImage*) * NpcBehaveMax);
	for (int i = 0; i < NpcBehaveMax; i++)
		imgs[i] = _imgIchi[i]->clone();
	imgCurr = imgs[index];

	rectNum = 1;
	rect = new iRect * [rectNum];
	for (int i = 0; i < rectNum; i++)
		rect[i] = new iRect();
}

Ichimondi::~Ichimondi()
{
	for (int i = 0; i < NpcIndexMax; i++)
	{
		delete _imgIchi;		
		_imgIchi = NULL;
	}
	for (int i = 0; i < rectNum; i++)
		delete rect[i];
	delete rect;
}

void Ichimondi::dead()
{
	state = (NpcBehave)(WalkNpcL + state % 2);
	imgs[state]->startAnimation(AnimationMgr::cbAniDead, this);
}

void Ichimondi::spawnItem()
{
	addProcItem(itemIndex, this->p);
}

iPoint initPos = iPointZero;
void Ichimondi::init(int index, iPoint p, int itemIndex)
{
	this->isActive = true;
	this->index = index;
	this->p = p;
	this->tp = p;
	this->itemIndex = itemIndex;
	state = IdleNpcL;

	for (int i = 0; i < rectNum; i++)
	{
		iRect* r = rect[i];
		r->size = iSizeMake(40, 40);
		r->origin = p;
	}
}
void Ichimondi::update(float dt)
{

	if (!isAppear)
	{
		if (containPoint(p,
			iRectMake(-map->off.x - 20, -map->off.y - 40,
				devSize.width + 40, devSize.height + 80)))
			isAppear = true;
	}
	else
	{
		isActive = containPoint(p,
			iRectMake(-map->off.x - 40, -map->off.y - 40,
				devSize.width + 80, devSize.height + 80));
	}

	if (v != iPointZero)
	{
		if (state == WalkNpcL || state == WalkNpcR)
		{
			if(initPos==iPointZero)
				initPos= p;
			int maxX;
			if (v.x > 0)
			{
				maxX = initPos.x + 100;
				state = WalkNpcR;				
				if (p.x > maxX)
					v.x = -1;
			}
			else if (v.x < 0)
			{
				maxX = initPos.y - 100;
				state = WalkNpcL;				
				if (p.x < maxX)
					v.x = 1;
			}
		}
			
	}
	if (state == WalkNpcL || state == WalkNpcR)
	{
		initPos = iPointZero;
		printf("v.x=%f\n", v.x);
		if (containPoint(player->p, iRectMake(p.x - 20, p.y - 40, 40, 40)))
		{
			player->addScore(score);
			audioPlay(snd_eff_thank);
			v = iPointZero;
			state = (NpcBehave)(AddItemNpcL + state % 2);			
		}
	}

	if (state == EscapeNpcL || state == EscapeNpcR)
	{
		imgs[state]->startAnimation();
		p += v * moveSpeed * 4 * dt;
	}
	else
		p += v * moveSpeed * dt;
	fixedUpdate(dt);
}

void Ichimondi::fixedUpdate(float dt)
{
	ProcNpc::fixedUpdate(dt);

	//ColliderUpdate
	for (int i = 0; i < rectNum; i++)
	{
		rect[i]->origin = iPointMake(
			p.x + map->off.x - rect[i]->size.width / 2,
			p.y + map->off.y - rect[i]->size.height);		
	}
}

#include "InputMgr.h"
bool Ichimondi::draw(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);
	imgCurr = imgs[state];
	imgCurr->paint(dt, p + off);	
	setRGBA(1, 1, 1, 1);

	if (getKeyStat(keyboard_delete))
#define DEBUG
#ifdef DEBUG
	for (int i = 0; i < rectNum; i++)
		drawRect(getRect());
#endif // _DEBUG
	return !isActive;
}


ImageInfo infoIchi[] =
{
	{
		"assets/NPC/Bondage_Sit_%02d.png",
		4, 1.0f, { -91 / 2, 0},
		0.18f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/NPC/Bondage_Release_%02d.png",
		4, 1.0f, { -91 / 2, 0},
		0.1f,
		1,
		{255, 0, 0, 255},
		AnimationMgr::cbAniNpcRelease,
	},
	{
		"assets/NPC/Walk_%02d.png",
		12, 1.0f, { -91 / 2, 0},
		0.1f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
	{
		"assets/NPC/AddItem_%02d.png",
		10, 1.0f, { -91 / 2, 0},
		0.12f,
		1,
		{255, 0, 0, 255},
		AnimationMgr::cbAniNpcSpawnItem,
	},
	{
		"assets/NPC/Salute_%02d.png",
		14, 1.0f, { -91 / 2, 0},
		0.08f,
		1,
		{255, 0, 0, 255},
		AnimationMgr::cbAniNpcSalute,
	},
	{
		"assets/NPC/Escape_%02d.png",
		8, 1.0f, { -91 / 2, 0},
		0.1f,
		0,
		{255, 0, 0, 255},
		NULL,
	},
};
