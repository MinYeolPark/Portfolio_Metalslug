#include "EffectMgr.h"

#include "ImgMgr.h"

// 3������ ����Ʈ, 100���� 
ProcEffect** _effect;
ProcEffect** effect;
int effectNum;

static iImage** _imgEffect = NULL;
ImageInfo imgEffectInfo[];
void loadProcEffect()
{
	_effect = new ProcEffect * [EffectIndexMax];
	for (int i = 0; i < EffectIndexMax; i++)
		_effect[i] = new ProcEffect[effectMax];
	effect = new ProcEffect * [EffectIndexMax * effectMax];
	effectNum = 0;

#if 0
	_effect = new ProcEffect * [EffectIndexMax];
	for (int i = 0; i < EffectIndexMax; i++)
	{
		_effect[i] = new ProcEffect[effectMax];
		for (int j = 0; j < effectMax; j++)
		{
			_effect[i][j].index = (EffectIndex)i;			
			_effect[i][j].imgs = _imgEffect[i]->clone();
		}
	}
	effect = new ProcEffect * [effectMax * EffectIndexMax];		
	effectNum = 0;
#endif
}

void freeProcEffect()
{
#if 1
	for (int i = 0; i < EffectIndexMax; i++)
	{
		delete _effect[i];
		delete effect[i];
	}
	delete _effect;
	delete effect;
#endif
#if 0
	for (int i = 0; i < EffectIndexMax; i++)
		delete _effect[i];
	delete _effect;
	delete effect;
#endif
}

void drawProcEffect(float dt, iPoint off)
{	
	for (int i = 0; i < effectNum; i++)
	{
		ProcEffect* e = effect[i];
		e->updateEffect(dt);
		if (e->drawEffect(dt, off))
		{
			effectNum--;
			effect[i] = effect[effectNum];
			i--;
		}
	}
}

void addProcEffect(int index, iPoint p)
{	
#if 1
	for (int i = 0; i < effectMax; i++)
	{
		ProcEffect* e = &_effect[index][i];
		if (e->isActive == false)
		{
			e->initEffect(index, p);
			effect[effectNum] = e;
			effectNum++;
			printf("add effect\n");
			return;
		}
	}
#endif
}

ProcEffect::ProcEffect()
{
	imgs = NULL;
	p = iPointZero;
	s = iSizeZero;

	index = (EffectIndex)0;
	isActive = false;
	delta = 0.f;
	_delta = 0.f;

	if (_imgEffect == NULL)
		_imgEffect = createImgEffect(imgEffectInfo, this);

	imgs = new iImage * [EffectIndexMax];
	memset(imgs, 0x00, sizeof(iImage*) * EffectIndexMax);
	for (int i = 0; i < EffectIndexMax; i++)
		imgs[i] = _imgEffect[i]->clone();
}

ProcEffect::~ProcEffect()
{
#if 0
	for (int i = 0; i < BulletIndexMax; i++)
		delete _imgEffect[i];
	delete _imgEffect;

	_imgEffect = NULL;
#endif
}

void ProcEffect::initEffect(int idx, iPoint p)
{
	isActive = true;
	this->index = (EffectIndex)idx;
	this->p = p;

	imgs[idx]->startAnimation(cbAniEffect, this);
}

void ProcEffect::updateEffect(float dt)
{
	//Add pattern
}

bool ProcEffect::drawEffect(float dt, iPoint off)
{
	setRGBA(1, 1, 1, 1);

	imgCurr = imgs[index];
	imgCurr->paint(dt, p + off);

#ifdef _DEBUG
	drawRect(p.x-imgCurr->tex->width/2, p.y-imgCurr->tex->height/2,
		imgCurr->tex->width / 2, imgCurr->tex->height / 2);
#endif
	setRGBA(1, 1, 1, 1);

	return !isActive;
}

void ProcEffect::freeEffect()
{
}

void ProcEffect::cbAniEffect(void* parm)
{
	printf("cbAniEffect\n");
	ProcEffect* e = (ProcEffect*)parm;
	e->isActive = false;	
}



ImageInfo imgEffectInfo[] =
{
	{
		"assets/Effect/Effect_%02d.png",
		10, 1.0f, { -16 / 2, 0},
		0.06f,
		1,
		{255, 0, 255, 255},
		TOP | LEFT,
		ProcEffect::cbAniEffect,
	},
	{
		"assets/Effect/Effect_%02d.png",
		10, 1.0f, { -16 / 2, 0},
		0.06f,
		1,
		{255, 0, 255, 255},
		TOP | LEFT,
		ProcEffect::cbAniEffect,
	},
	{
		"assets/Effect/Explosion_%02d.png",
		21, 1.0f, { -48 / 2, 0},
		0.08f,
		1,
		{255, 0, 0, 255},
		TOP | LEFT,
		ProcEffect::cbAniEffect,
	},
};