#include "Koopas.h"
#include "Mario.h"
#include "Utils.h"
#include "FireBall.h"
CKoopas::CKoopas()
{
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	if (!isDead)
	{
		vy += 0.002f * dt;
		vx = -0.003 * dt;
	}
	else if (state == KOOPAS_SHELL_MOVE)
	{
		vx = -0.01 * dt;
	}
	if (isDead && state!=KOOPAS_SHELL_MOVE)
	{
		vx = 0;
		if (timeframe <= 30)
			timeframe++;

	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	if (timeframe <= 30)
	{
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
			if (ny < 0) y += min_ty * dy + ny * 0.4f;
			if (nx != 0) vx = -vx;
			if (ny != 0)
			{
				vy = 0;
			}

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];
				if (dynamic_cast<FireBall*>(e->obj))
				{
					isDead = true;
					vy = -KOOPAS_DEFLECT_FORCE;
				}
			}
		}
	}

}

void CKoopas::Render()
{
	if (timeframe > 30) return;
	int ani = KOOPAS_ANI_WALKING_LEFT;
	if (vx < 0) ani = KOOPAS_ANI_WALKING_LEFT;
	else if (vx > 0) ani = KOOPAS_ANI_WALKING_RIGHT;
	if (state == KOOPAS_STATE_DIE) {
		ani = KOOPAS_ANI_WALKING_LEFT;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		isDead = true;
		break;
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_SHELL_MOVE:
		vx = -0.1;
		break;
	}
}
