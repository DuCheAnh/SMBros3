#include "coin.h"

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (!gotnomed)
	{
		l = x;
		t = y;
		r = x + BBCOIN_WIDTH;
		b = y + BBCOIN_HEIGHT;
	}
}
void CCoin::Consumed()
{
	gotnomed = true;
}

void CCoin::Render()
{
	if (!gotnomed)
	animation_set->at(DEFAULT_ANI)->Render(x, y);
}