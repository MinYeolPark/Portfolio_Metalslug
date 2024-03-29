﻿#include "ProcField.h"
#include "ProcPlayer.h"

#include "UIMgr.h"
#include "ImgMgr.h"
#include "InputMgr.h"
int mapNum[] =
{
	6,
};
LayerData layerData[layerNum] =
{
	{
		{0,90},
		0.2f,
	},
	{
		{0,105},
		0.5f,
	},
	{
		{0,160},
		0.8f,
	},
};
ProcMap* map;
MapData mapData[];
ImageInfo mapImageInfo[];
ImageInfo layerImageInfo[];
ProcMap::ProcMap(int stage)
{
	//Map
	this->stage = stage;
	maxW = 0;
	maxY = 0;
	isClipped = false;

	imgs = new iImage * [mapNum[stage]];
	imgs = createSingleImage(mapImageInfo,	mapNum[stage], this);	
	if(stage==0)
		imgs[3]->alpha = 0.0f;

	//Layer	
	imgsLayer = new iImage * [layerNum];
	imgsLayer = createSingleImage(layerImageInfo, layerNum, this);	
}

ProcMap::~ProcMap()
{
	for (int i = 0; i < mapNum[stage]; i++)	
		delete imgs[i];
	delete imgs;

	for (int j = 0; j < layerNum; j++)
		delete imgsLayer[j];
	delete imgsLayer;

}

void ProcMap::init(int stage)
{
	audioStop(snd_bgm_title);
	audioStop(snd_bgm_select);
	
	if (stage == 0)	//stage 1
	{
		audioPlay(snd_bgm_stage1);
		audioPlay(snd_nar_m1_start);

		showMissionPopup(true, 0.000001f);
	}
	for (int i = 0; i < mapNum[stage]; i++)
	{
		Texture* t = imgs[i]->tex;
		maxW += t->width;
	}	
	//#issue
	maxW = 1176 + 786 + 550 + 1008 + 331;
	
	maxY = new int[maxW];	
	for (int j = 0; j < mapNum[stage]; j++)
	{
		md = &mapData[j];
		for (int k = 0; k < md->pCount; k++)
		{
			iPoint sp = md->point[k];
			iPoint ep = md->point[1 + k];

			for (int m = sp.x; m < ep.x; m++)
			{
				float a = (m - sp.x) / (ep.x - sp.x);
				maxY[m] = sp.y + (ep.y - sp.y) * a;
			}
		}
	}		
	_off = 
	off = iPointZero;
	offMax = iPointMake(0, 18);
	offMin = iPointMake(devSize.width - maxW, 0);	
}

void ProcMap::update(float dt)
{	
	if (isClipped)
		return;

	//viewChange
	if (player->p.x < 1200 - devSize.width / 2)
		_off.y = 0;
	else if (player->p.x < 1200)
	{
		float range = devSize.width / 2;
		_off.y = linear((player->p.x - 1050 + range) / range, 0.0f, offMax.y);
	}
	else
	{
		_off.y = offMax.y;
	}
	//Camera Move
	float x = player->p.x + _off.x;
	if (x < devSize.width / 3)
		move(iPointMake((devSize.width / 3 - x), _off.y));
	else if (x > devSize.width * 2 / 3)
		move(iPointMake((devSize.width * 2 / 3 - x), _off.y));

#if 1
	static bool first = true;
	if (first)
	{
		first = false;
		off = _off;
	}
#endif
	
#if 0
	if (off.x < _off.x)
	{
		off.x += speed;
		if (off.x > _off.x)
			off.x = _off.x;
	}
	else if (off.x > _off.x)
	{
		off.x -= speed;
		if (off.x < _off.x)
			off.x = _off.x;
	}
	if (off.y < _off.y)
	{
		off.y += speed;
		if (off.y > _off.y)
			off.y = _off.y;
	}
	else if (off.y > _off.y)
	{
		off.y -= speed;
		if (off.y < _off.y)
			off.y = _off.y;
	}
#else
	movePoint(off, off, _off, player->moveSpeed * dt);
#endif
}
#include "InputMgr.h"
void ProcMap::paint(float dt)
{
	setRGBA(1, 1, 1, 1);
	iPoint p;
	//Layer
	for (int i = 0; i < layerNum; i++)
	{
		ld = &layerData[i];
		Texture* t = imgsLayer[i]->tex;
		p.x = ld->offSet.x + off.x * ld->rate;
		p.y = ld->offSet.y;
		while (p.x < -t->width * layerImageInfo[i].s)
			p.x += t->width * layerImageInfo[i].s;
		for (int j = 0; j < 3; j++)
		{
			imgsLayer[i]->paint(dt, 
				iPointMake(p.x + t->width * j * layerImageInfo[i].s, p.y));
		}
	}
	for (int i = 0; i < mapNum[stage]; i++)
	{
		md = &mapData[i];
		imgs[i]->paint(dt, md->p + off);
	}	
#ifdef DEBUG
	setLineWidth(2);
	for (int j = 0; j < mapNum[stage]; j++)
	{
		md = &mapData[j];
		for (int i = 0; i < md->pCount - 1; i++)
		{
			drawLine(md->point[i].x + map->off.x, md->point[i].y + map->off.y,
				md->point[i + 1].x + map->off.x, md->point[i + 1].y + map->off.y);
		}
	}
#endif // _DEBUG
	setRGBA(1, 1, 1, 1);
}
bool ProcMap::move(iPoint mp)
{
	_off += mp;
	if (_off.x < offMin.x)
		_off.x = offMin.x;
	else if (_off.x > offMax.x)
		_off.x = offMax.x;	

	if (_off.y < offMin.y)
		_off.y = offMin.y;
	else if (_off.y > offMax.y)
		_off.y = offMax.y;
	return _off.x == offMax.x || _off.y == offMax.y;
}


MapData mapData[] = {
	{
		{0, 0},
		{{1176 * 0, 170}, {155,145}, {280,175}, {415, 160}, {560, 180}, {1176 * 1, 180}},
		6,
	},
	{
		{1176, -17},
		{{1176 * 1, 180}, {1176 * 1.3, 180}, {1176 * 1.6, 180}, {1176 + 786, 180}},
		4,
	},
	{
		{1176 + 786, -17},
		{{(1176 + 786) * 1, 180}, {(1176 + 786 + 686) * 1, 180} },
		2,
	},
	{
		{1176 + 786 + 550, -17},
		{{(1176 + 786 + 550) * 1, 180}, {(1176 + 786 + 550 + 1008), 180}},
		2,
	},
	{
		{1176 + 786 + 686 + 872, -17},
		{{(1176 + 786 + 550 + 1008), 180}, {(1176 + 786 + 550 + 1008), 180}},
		2,
	},
	{
		{1176 + 786 + 686 + 872, -48},
		{{(1176 + 786 + 686) * 1, 180}, {(1176 + 786 + 550 + 1008 + 331), 180}},
		2,
	},
};
ImageInfo mapImageInfo[] =
{
	{
		"assets/Map/Map_00.png",
		1, 1.f, { 0 , -224},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/Map/Map_01.png",
		1, 1.f, { 0 , -224},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/Map/Map_02.png",
		1, 1.f, { 0 , -224},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/Map/Map_03.png",
		1, 1.f, { 0 , -224},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/Map/Map_04.png",
		1, 1.f, { 0 , -224},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/Map/Final_%02d.png",
		4, 1.f, { 0 , -224},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
};

ImageInfo layerImageInfo[] =
{
	{
		"assets/map/BG_00.png",
		1, 1.f, { -164 / 2 , 0},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/map/BG_01.png",
		1, 1.f, { -164 / 2 , 0},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/map/BG_02.png",
		1, 1.f, { -164 / 2 , 0},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
};
ImageInfo mapObjectInfo[] =
{
	{
		"assets/map/Apple.png",
		1, 1.f, { -164 / 2 , 0},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		"assets/map/Wall_%02d.png",
		3, 1.f, { -104 / 2 , 0},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
	{
		//{2238, 196}
		"assets/map/Final_%02d.png",
		3, 1.f, { -304 / 2 , -208},
		0.1f,
		0,
		{248,0,248,255},
		NULL,
	},
};
