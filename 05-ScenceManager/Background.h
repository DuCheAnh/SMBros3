#pragma once
#include "GameObject.h"
#define BACKGROUND_BBOX_WIDTH  0
#define BACKGROUND_BBOX_HEIGHT 0
class CBackground : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b) {};
};

