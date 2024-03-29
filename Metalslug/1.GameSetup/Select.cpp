#include "Select.h"

#include "UIMgr.h"
#include "InputMgr.h"
#include "ImgMgr.h"

#include "Loading.h"
#include "Proc.h"
#include "ProcPlayer.h"

Texture* selectmap;
iImage** imgSelectBtn;
iImage** imgSelectPosBtn;
Texture* texShutter;
iImage* imgTop;
iImage* imgBot;
int selectedBtn;
bool confirm = false;
iPoint sp[4] =
{
	{16 + 70 * 0, devSize.height + 70},
	{16 + 70 * 1, devSize.height + 70},
	{16 + 70 * 2, devSize.height + 70},
	{16 + 70 * 3, devSize.height + 70},
};
iPoint shutterP[4];
float shutterRate[4];
void loadSelect()
{
	loadProcPlayer();
	audioStop(snd_bgm_title);
	audioPlay(snd_bgm_select);
	
	selectmap = createImage("assets/CharSelect/CharacterSelectBg.png");
		
	imgSelectBtn = new iImage * [CharacterIndexMax];
#if 1
	imgSelectPosBtn = new iImage * [CharacterIndexMax];	
	for (int j = 0; j < CharacterIndexMax; j++)
	{
		iImage* _imgSelectBtn = new iImage();
		iImage* _imgSelectPos = new iImage();
		for (int i = 0; i < 3; i++)
		{
			Texture* t = createImage("assets/CharSelect/CharacterSelect_%d%d.png", i, j);
			_imgSelectBtn->addObject(t);
			freeImage(t);

			t->width *= devSize.width / selectmap->width;
			t->potWidth *= devSize.width / selectmap->width;
			t->height *= devSize.height / selectmap->height;
			t->potHeight *= devSize.height / selectmap->height;

			t=createImage("assets/CharSelect/P1_%02d.png", j);
			_imgSelectPos->addObject(t);
			freeImage(t);

			t->width *= devSize.width / selectmap->width;
			t->potWidth *= devSize.width / selectmap->width;
			t->height *= devSize.height / selectmap->height;
			t->potHeight *= devSize.height / selectmap->height;
		}
		_imgSelectBtn->position = iPointMake(15 + (_imgSelectBtn->tex->width + 8) * j, 70);

		iPoint p[4] = {
			{_imgSelectBtn->position.x + _imgSelectBtn->tex->width / 2 + 3, _imgSelectBtn->position.y - 14},
			{_imgSelectBtn->position.x + _imgSelectBtn->tex->width / 2 - 6 , _imgSelectBtn->position.y - 14},
			{_imgSelectBtn->position.x + _imgSelectBtn->tex->width / 2 + 3, _imgSelectBtn->position.y - 14},
			{_imgSelectBtn->position.x + _imgSelectBtn->tex->width / 2 - 8, _imgSelectBtn->position.y - 14}
		};
		_imgSelectPos->position = p[j];
		_imgSelectPos->anc = VCENTER | HCENTER;

		imgSelectBtn[j] = _imgSelectBtn;
		imgSelectPosBtn[j] = _imgSelectPos;
	}	
#if 1
	texShutter = createImage("assets/CharSelect/Shutter.png");	
	imgTop = player->topImgs[PlayerIdle];	
	imgTop->_aniDt = 0.5f;
	imgTop->alpha = 0.f;
	imgBot = player->botImgs[PlayerIdle];		
	imgBot->alpha = 0.f;
#endif
	for (int i = 0; i < 4; i++)
	{
		shutterP[i] = sp[i];
		int r = rand() % 5 + 3;
		shutterRate[i] = r;
	}
	selectedBtn = 0;
#endif
}
void freeSelect()
{
	freeProcPlayer();
	for (int i = 0; i < CharacterIndexMax; i++)
	{
		delete imgSelectBtn[i];
		delete imgSelectPosBtn[i];
	}
	delete imgSelectBtn;
	delete imgSelectPosBtn;
	delete selectmap;
}


void drawSelect(float dt)
{
	setRGBA(1, 1, 1, 1);
			
	for (int i = 0; i < CharacterIndexMax; i++)
	{
		if (!confirm)
			imgSelectBtn[i]->frame = (i == selectedBtn);			
		imgSelectBtn[i]->paint(dt, iPointZero, iImageTypePop);
	}
#if 1
	for (int i = 0; i < CharacterIndexMax; i++)
	{			
		drawImage(texShutter, shutterP[i]);
		iPoint p = shutterP[i];		
		imgBot->paint(dt, iPointMake(p.x + 30, p.y + 72), iImageTypePop);
		imgTop->paint(dt, iPointMake(p.x  + 30, p.y + 50), iImageTypePop);
		if (isLoaded)
		{
			iPoint tp = { shutterP[i].x, -70 };			
			movePoint(shutterP[i], shutterP[i], tp, shutterRate[i]);
		}

		if (confirm)
		{			
			imgBot->alpha = 1.f;
			imgTop->alpha = 1.f;
			iPoint tp = { shutterP[selectedBtn].x, 70 };			
			if (movePoint(shutterP[selectedBtn], shutterP[selectedBtn], tp, shutterRate[i]))
				setLoading(GameStateProc, 2, freeSelect, loadProc);
		}
	}
	drawImage(selectmap, 0, 0,
		devSize.width / selectmap->width,
		devSize.height / selectmap->height, TOP | LEFT);

	for (int i = 0; i < CharacterIndexMax; i++)
	{
		if (selectedBtn == i)
			imgSelectPosBtn[i]->paint(dt, iPointZero, iImageTypePop);
	}
#endif

	if (!confirm)
	{
		if (getKeyDown(keyboard_right))
		{
			selectedBtn++;
			if (selectedBtn > CharacterIndexMax - 1)
				selectedBtn = 0;
		}
		if (getKeyDown(keyboard_left))
		{
			selectedBtn--;
			if (selectedBtn < 0)
				selectedBtn = CharacterIndexMax - 1;
		}
		if (getKeyDown(keyboard_enter))
		{
			confirm = true;
			imgSelectBtn[selectedBtn]->frame = 2;

			if(selectedBtn==0)
				audioPlay(snd_nar_select_eri);
			else if(selectedBtn==1)
				audioPlay(snd_nar_select_eri);
		}
	}	
	setRGBA(1, 1, 1, 1);
}
bool keySelect(iKeyState stat, iPoint p)
{
#if 1
	int i, j = -1;
	switch (stat) {

	case iKeyStateBegan:
		i = selectedBtn;
		if (i == -1) break;
		if (i == 0)
		{
			confirm = true;
			imgSelectBtn[0]->frame = 2;
		}
		else if (i == 1)
		{
			confirm = true;
			imgSelectBtn[1]->frame = 2;
			printf("eri\n");
		}
		else if (i == 2)
		{
			confirm = true;
			imgSelectBtn[2]->frame = 2;
			printf("tama\n");
		}
		else
		{
			confirm = true;
			imgSelectBtn[3]->frame = 2;
			printf("fio\n");
		}
		break;
	case iKeyStateMoved:
		if (confirm) break;

		for (i = 0; i < CharacterIndexMax; i++)
		{
			if (containPoint(p, imgSelectBtn[i]->rect()))
			{
				j = i;
				break;
			}
		}
		if (selectedBtn != j)
		{
			;
			selectedBtn = j;
		}
		break;
	case iKeyStateEnded:
		break;
	}
#endif
	return true;
}
