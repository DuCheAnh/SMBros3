#include "Platform.h"

Platform::Platform(float l, float t, float r, float b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

Platform::~Platform()
{

}

void Platform::Render()
{
	RenderBoundingBox();
}

void Platform::DisablePlatform(int height)
{
	disabled =!disabled;
	player_height = height;
}
void Platform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
}


void Platform::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!disabled)
	{
		l = left;
		t = top;
		r = right;
		b = bottom;
	}
	else
	{
		l = left;
		t = top - player_height;
		r = right;
		b = bottom - player_height;
	}
}