#include "Mysterious.h"
#include <time.h>
#include "Utils.h"
void CMysterious::Render()
{
	
	animation_set->at(state)->Render(x, y);
	//RenderBoundingBox();
}

CMysterious::CMysterious()
{
	state = STATE_DEFAULT;
}
void CMysterious::GotPopped()
{
	state = STATE_POPPED;
}
void CMysterious::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}