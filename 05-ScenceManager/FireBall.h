#pragma once

#include <vector>

#include "GameObject.h"


#define BULLET_BBOX_WIDTH 6
#define BULLET_BBOX_HEIGHT 6
#define BULLET_VX 0.2
#define BULLET_GRAVITY 0.003f 
#define BOUNCING_FORCE 0.01f


#define ANI_BULLET	"ani-fire-ball"
class FireBall :public CGameObject
{
public:

	FireBall(float x, float y, int direction);
	~FireBall() {  };
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void PreLoadAni();
	virtual void Update(DWORD dt, std::vector<LPGAMEOBJECT>* coObjects = NULL);
};

