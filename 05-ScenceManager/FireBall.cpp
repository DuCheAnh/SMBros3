#include "FireBall.h"
#include "PlayScence.h"

FireBall::FireBall(float x, float y, int direction)
{
	PreLoadAni();
	this->x = x;
	this->y = y;
	vx = direction * FIREBALL_VX;

}

void FireBall::Render()
{
	if(!fireOut)
		animation_set->at(0)->Render(x, y);
}

void FireBall::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FIREBALL_BBOX_WIDTH;
	b = y + FIREBALL_BBOX_HEIGHT;
}

void FireBall::PreLoadAni()
{
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	LPANIMATION_SET ani_set = animation_sets->Get(99);

	SetAnimationSet(ani_set);
}

void FireBall::Update(DWORD dt, std::vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += FIREBALL_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;
	coEvents.clear();
	if (!fireOut)
		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;
		if (ny < 0) vy = -BOUNCING_FORCE*dt;


		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (e->nx != 0)
			{
				fireOut = true;
			}
		}

	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

