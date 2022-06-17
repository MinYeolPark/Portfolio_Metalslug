#include "Kessie.h"

#include "ImgMgr.h"
#include "EffectMgr.h"
#include "BulletMgr.h"
#include "EnemyMgr.h"
ImageInfo imgKessieInfo[];
static iImage** _imgKessie = NULL;
Kessie::Kessie()
{
	memset(imgBase, 0x00, sizeof(imgBase));	
	memset(imgLeftCrater, 0x00, sizeof(imgLeftCrater));
	memset(imgRightCrater, 0x00, sizeof(imgRightCrater));
	memset(imgLeftFan, 0x00, sizeof(imgLeftFan));
	memset(imgRightFan, 0x00, sizeof(imgRightFan));
	imgHead = 0;
	memset(imgLeftBlast, 0x00, sizeof(imgLeftBlast));
	memset(imgRightBlast, 0x00, sizeof(imgRightBlast));
		
	isActive = true;
	isHeadOpen = false;
	effDt = 0.f;
	_effDt = 3.0f;
	aiDt = 0.f;
	_aiDt = 5.f;
	_hp = 5000.f;
	hpLeft = 1500.f;
	hpRight = 1500.f;

	if (_imgKessie == NULL)
		_imgKessie = createSingleImage(imgKessieInfo, 21, this);

	for(int i=0;i<8;i++)
		imgBase[i] = _imgKessie[i];
	imgLeftCrater[0] = _imgKessie[8]->clone();
	imgLeftCrater[1] = _imgKessie[9]->clone();
	imgLeftCrater[2] = _imgKessie[10]->clone();
	imgRightCrater[0] = _imgKessie[11]->clone();
	imgRightCrater[1] = _imgKessie[12]->clone();
	imgRightCrater[2] = _imgKessie[13]->clone();
	imgLeftFan[0] = _imgKessie[14]->clone();
	imgLeftFan[1] = _imgKessie[15]->clone();
	imgRightFan[0] = _imgKessie[16]->clone();
	imgRightFan[1] = _imgKessie[17]->clone();
	imgHead = _imgKessie[18]->clone();	
	imgHead->stopAnimation();
	imgLeftBlast[0] = _imgKessie[19]->clone();	
	imgRightBlast[0] = _imgKessie[19]->clone();
	imgRightBlast[0]->reverse = REVERSE_WIDTH;	
	imgLeftBlast[1] = _imgKessie[20]->clone();
	imgRightBlast[1] = _imgKessie[20]->clone();
	imgRightBlast[1]->reverse = REVERSE_WIDTH;
}

Kessie::~Kessie()
{
	for (int i = 0; i < 13; i++)
		delete _imgKessie[i];
	delete _imgKessie;
	
	_imgKessie = NULL;
}

void Kessie::init()
{
	p = iPointMake(230, 150);
}

void Kessie::update(float dt)
{	
	aiDt += dt;
	if (aiDt > _aiDt)
	{
		aiDt -= _aiDt;
		addProcEnemy(ArMelee, { p.x + 100, p.y - 100 }, iPointMake(-1, 0), AI::enemyAI0);
	}
	if (!isHeadOpen)
	{
		imgHead->startAnimation();
		isHeadOpen = true;
	}
	else
	{
		//spawn ArabMelee
		//moving pattern
	}
	fixedUpdate(dt);
}

void Kessie::fixedUpdate(float dt)
{

}

bool Kessie::draw(float dt, iPoint off)
{
	imgHead->paint(dt, { p.x, p.y - 90 });
		
	imgBase[0]->paint(dt, p + off);

	imgLeftFan[0]-> paint(dt, { p.x + off.x - 96, p.y + off.y - 48 });
	imgRightFan[1]->paint(dt, { p.x + off.x + 96, p.y + off.y - 48 });
	imgLeftCrater[0]->paint(dt, { p.x + off.x - 88, p.y + off.y - 25 });
	imgRightCrater[1]->paint(dt, { p.x + off.x + 88, p.y + off.y - 25 });

	imgLeftBlast[0]->paint(dt, { p.x + off.x - 88, p.y + off.y + imgLeftBlast[0]->tex->height - 40 });
	imgRightBlast[1]->paint(dt, { p.x + off.x + 88, p.y + off.y + imgRightBlast[1]->tex->height - 40 } );
#ifdef _DEBUG	
	drawDot(p + off);	
	drawDot({ p.x + off.x - 88, p.y + off.y - 25 });
	drawDot({ p.x + off.x + 88, p.y + off.y - 25 });
	drawRect(p.x + off.x - 88,
		p.y + off.y + imgLeftBlast[0]->tex->height - 40,
		imgLeftBlast[0]->tex->width,
		imgLeftBlast[0]->tex->height);
#endif // DEBUG

	
	return !isActive;
}

void Kessie::free()
{
}


ImageInfo imgKessieInfo[] =
{
	{
		"assets/Kessie/Base_00.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},	
	{
		"assets/Kessie/Base_01.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Base_02.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Base_03.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Base_04.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Base_05.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Base_06.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Base_07.png",
		1, 1.f, { -272 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/CraterLeft_%02d.png",
		3, 1.f, { -32 / 2 , 0},
		0.1f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/CraterLeft_70_%02d.png",
		15, 1.f, { -32 / 2 , 0},
		0.1f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/CraterLeft_Dead_%02d.png",
		6, 1.f, { -104 / 2 , 0},
		0.1f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/CraterRight_00.png",
		1, 1.f, { -32 / 2 , 0},
		0.1f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/CraterRight_70_%02d.png",
		15, 1.f, { -32 / 2 , 0},
		0.1f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/CraterRight_Dead_%02d.png",
		6, 1.f, { -54 / 2 , 0},
		0.1f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/FanLeft_%02d.png",
		3, 1.f, { -64 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/FanLeft_Rage_%02d.png",
		6, 1.f, { -64 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/FanRight_%02d.png",
		3, 1.f, { -64 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/FanRight_Rage_%02d.png",
		6, 1.f, { -64 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Head_%02d.png",
		7, 1.f, { -96 / 2 , 0},
		0.18f,
		1,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Blast_Idle_%02d.png",
		4, 1.f, { -48 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
	{
		"assets/Kessie/Blast_%02d.png",
		47, 1.f, { -48 / 2 , 0},
		0.06f,
		0,
		{0,248,0,255},
		NULL,
	},
};