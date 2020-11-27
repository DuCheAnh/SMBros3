#pragma once

#include <vector>

#include "GameObject.h"


#define FIREBALL_BBOX_WIDTH 6
#define FIREBALL_BBOX_HEIGHT 6
#define FIREBALL_VX 0.2
#define FIREBALL_GRAVITY 0.003f 
#define BOUNCING_FORCE 0.012f


class FireBall :public CGameObject
{
public:
	bool fireOut = false;
	FireBall(float x, float y, int direction);
	~FireBall() {  };
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void PreLoadAni();
	virtual void Update(DWORD dt, std::vector<LPGAMEOBJECT>* coObjects = NULL);
};

