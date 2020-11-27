#include "Goomba.h"
#include "Mario.h"
#include "FireBall.h"
CGoomba::CGoomba()
{
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + GOOMBA_BBOX_WIDTH;

	if (state == GOOMBA_STATE_DIE)
		bottom = y + GOOMBA_BBOX_HEIGHT_DIE;
	else 	
		bottom = y + GOOMBA_BBOX_HEIGHT;
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{	
		CGameObject::Update(dt, coObjects);

		vy += 0.002f * dt;

		if (isDead)
		{
			vx = 0;
			if (timeframe <= 30)
				timeframe++;

		}
		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		// turn off collision when die 
		if (timeframe<=30)
			CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() == 0)
		{
			x += dx;
			y += dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny;
			float rdx = 0;
			float rdy = 0;

			// TODO: This is a very ugly designed function!!!!
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;

			if (nx != 0) vx = -vx;
			if (ny != 0) vy = 0;
			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<FireBall*>(e->obj))
				{
					isDead = true;
					vy = -GOOMBA_DEFLECT_FORCE;
				}
			}


		}

}

void CGoomba::Render()
{
	if (timeframe > 30)
		return;
	int ani = GOOMBA_ANI_WALKING;
	if (state == GOOMBA_STATE_DIE) {
		ani = GOOMBA_ANI_DIE;
	}

	animation_set->at(ani)->Render(x,y);

	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			//y += GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE + 1;
			isDead=true;
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
	}
}
