#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Mysterious.h"
#include "Portal.h"
#include "coin.h"
#include "Platform.h"
#include "Koopas.h"
#include "FireBall.h"
#include "PlayScence.h"
#include "Indestructible.h"
#include "Platform.h"
CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_FIRE;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);

	start_x = x; 
	start_y = y; 
	this->x = x; 
	this->y = y; 
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	if (!isFlying) vx = vx + a * dt;
	CGameObject::Update(dt);
	if (!isFlying) vy += (MARIO_GRAVITY / slow_fall_mu1) * dt;
	// Simple fall down
	if (isOnFloor) 
	{
		mul = 1;
	}

	if (abs(vx) > maximum_speed_mul * MARIO_WALKING_SPEED)
	{
		vx = nx*maximum_speed_mul * MARIO_WALKING_SPEED;
	}
	if (abs(vx) >= 4 * MARIO_WALKING_SPEED && level==MARIO_LEVEL_RACOON)
	{
		if (state == MARIO_STATE_JUMP)
		{
			isFlying = true;
			if (nx > 0) flyingRight = true;
			else flyingRight = false;
		}
	}
	if (isFlying)
	{
		maximum_speed_mul = 2;
		vy += (MARIO_GRAVITY/20 ) * dt;
		if (state == MARIO_STATE_JUMP)
		{
			vy = -0.005f * dt;
			if (flyingRight)
				vx = MARIO_WALKING_SPEED / 3 * dt;
			else vx = -MARIO_WALKING_SPEED / 3 * dt;
		}
		isOnFloor = false;
		mul = 0;
		
	}
	if (vy > 0 && isFlying)
	{
		isFlying = false;
		SetState(MARIO_STATE_JUMP);
	}
	if (isSlowFalling)
	{
		if (timeframe < 40)
		{
			++timeframe;
			slow_fall_mu1 = 50;
		}
		if (timeframe >= 40)
		{
			timeframe = 0;
			slow_fall_mu1 = 1;
			isSlowFalling = false;
		}
	}
	if (state == MARIO_STATE_JUMP && !isOnFloor && mul == 0 && vy > 0 && level==MARIO_LEVEL_RACOON)
	{
 		timeframe = 0;
		isSlowFalling = true;
	}


	if (state == MARIO_STATE_IDLE)
	{
		a = -(vx / abs(vx)) * MARIO_ACCELERATION / 3;
		if (abs(vx) < 0.05f)
		{
			a = 0;
			vx = 0;
		}
	}
	
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	
	// turn off collision when die 
	if (state!=MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);


	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
	if (coEvents.size()==0)
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
		
		if (ny < 0)
		{
			isOnFloor = true;
			isSlowFalling = false;
			slow_fall_mu1 = 1;
			timeframe = 0;
		}
		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 
		
		//isACoint represent a coin or a platform that will be disable until ny>0
		bool isACoin = false;
		// block every object first!
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CCoin*>(e->obj)) isACoin = true;
			if (dynamic_cast<Platform*>(e->obj))
			{
				Platform* pl = dynamic_cast<Platform*>(e->obj);
				if (e->ny > 0)
				{
					pl->DisablePlatform(height);
					isACoin = true;
				}
			}
			if (dynamic_cast<CKoopas*>(e->obj))
			{
				CKoopas* koopa = dynamic_cast<CKoopas*>(e->obj);
				if (koopa->GetState() == KOOPAS_STATE_SHELL_MOVING && e->nx!=0)
				{
					isACoin = true;
					if (untouchable == 0)
					{
						if (koopa->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			}
		}
		if (!isACoin)
		{

			x += min_tx * dx + nx * 0.4f;
			y += min_ty * dy + ny * 0.4f;

			if (nx != 0) vx = 0;
			if (ny != 0) vy = 0;
		}


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			
			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba  
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState()!= GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable==0)
					{
						if (goomba->GetState()!=GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else 
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			} // if Goomba
			else if (dynamic_cast<CKoopas *>(e->obj)) // if e->obj is Goomba 
			{
				CKoopas* koopa = dynamic_cast<CKoopas*>(e->obj);
				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (koopa->GetState() != KOOPAS_STATE_DIE)
					{
						koopa->SetState(KOOPAS_STATE_SHELL);
						//vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				if (e->nx != 0)
				{
					if (koopa->GetState() == KOOPAS_STATE_SHELL)
					{
						koopa->SetState(KOOPAS_STATE_SHELL_MOVING);
					}
					
				}
			}
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else if (dynamic_cast<CMysterious*>(e->obj))
			{
				CMysterious* b = dynamic_cast<CMysterious*>(e->obj);
				if (e->ny > 0) b->GotPopped();
				mul = 0;
			}
			else if (dynamic_cast<CCoin*>(e->obj))
			{
				CCoin* c = dynamic_cast<CCoin*>(e->obj);
				c->Consumed();
			}
		
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CMario::Render()
{
	int ani = -1;
	if (state == MARIO_STATE_DIE)
		ani = MARIO_ANI_DIE;
	else
	if (level == MARIO_LEVEL_BIG)
	{
		StateRenderding(ani, MARIO_ANI_BIG_IDLE_RIGHT, MARIO_ANI_BIG_IDLE_LEFT, MARIO_ANI_BIG_WALKING_RIGHT,
			MARIO_ANI_BIG_WALKING_LEFT, MARIO_ANI_BIG_BREAK_RIGHT, MARIO_ANI_BIG_BREAK_LEFT,MARIO_ANI_BIG_JUMP_RIGHTUP,
			MARIO_ANI_BIG_JUMP_RIGHTDN, MARIO_ANI_BIG_JUMP_LEFTUP, MARIO_ANI_BIG_JUMP_LEFTDN,MARIO_ANI_BIG_MAXSPD_RIGHT,
			MARIO_ANI_BIG_MAXSPD_LEFT);
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		StateRenderding(ani, MARIO_ANI_SMALL_IDLE_RIGHT, MARIO_ANI_SMALL_IDLE_LEFT, MARIO_ANI_SMALL_WALKING_RIGHT,
			MARIO_ANI_SMALL_WALKING_LEFT, MARIO_ANI_SMALL_BREAK_RIGHT, MARIO_ANI_SMALL_BREAK_LEFT, MARIO_ANI_SMALL_JUMP_RIGHTUP,
			MARIO_ANI_SMALL_JUMP_RIGHTDN, MARIO_ANI_SMALL_JUMP_LEFTUP, MARIO_ANI_SMALL_JUMP_LEFTDN, MARIO_ANI_SMALL_MAXSPD_RIGHT,
			MARIO_ANI_SMALL_MAXSPD_LEFT);
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		StateRenderding(ani, MARIO_ANI_FIRE_IDLE_RIGHT, MARIO_ANI_FIRE_IDLE_LEFT, MARIO_ANI_FIRE_WALKING_RIGHT,
			MARIO_ANI_FIRE_WALKING_LEFT, MARIO_ANI_FIRE_BREAK_RIGHT, MARIO_ANI_FIRE_BREAK_LEFT, MARIO_ANI_FIRE_JUMP_RIGHTUP,
			MARIO_ANI_FIRE_JUMP_RIGHTDN, MARIO_ANI_FIRE_JUMP_LEFTUP, MARIO_ANI_FIRE_JUMP_LEFTDN, MARIO_ANI_FIRE_MAXSPD_RIGHT,
			MARIO_ANI_FIRE_MAXSPD_LEFT);
		if (state == SHOOT_FIREBALL && nx > 0)
			ani = MARIO_ANI_FIRE_SHOOT_RIGHT;
		else if (state == SHOOT_FIREBALL && nx < 0)
			ani = MARIO_ANI_FIRE_SHOOT_LEFT;
	}
	else if (level == MARIO_LEVEL_RACOON)
	{
		StateRenderding(ani, MARIO_ANI_RACOON_IDLE_RIGHT, MARIO_ANI_RACOON_IDLE_LEFT, MARIO_ANI_RACOON_WALKING_RIGHT,
			MARIO_ANI_RACOON_WALKING_LEFT, MARIO_ANI_RACOON_BREAK_RIGHT, MARIO_ANI_RACOON_BREAK_LEFT, MARIO_ANI_RACOON_JUMP_RIGHTUP,
			MARIO_ANI_RACOON_JUMP_RIGHTDN, MARIO_ANI_RACOON_JUMP_LEFTUP, MARIO_ANI_RACOON_JUMP_LEFTDN, MARIO_ANI_RACOON_MAXSPD_RIGHT,
			MARIO_ANI_RACOON_MAXSPD_LEFT);
	}
	int alpha = 255;
	if (untouchable) alpha = 128;
	animation_set->at(ani)->Render(x, y, alpha);
	RenderBoundingBox();
}


void CMario::StateRenderding(int &ani,int idleR, int idleL, int walkingR, int walkingL, int breakR, int breakL,
							int jumpRup, int jumpRdn, int jumpLup, int jumpLdn,int maxspdR, int maxspdL)
{
	if (mul!=0)
	{
		if (vx == 0)
		{
			if (nx > 0) ani = idleR;
			else ani = idleL;
		}
		else if (vx >= 4 * MARIO_WALKING_SPEED)
			ani = maxspdR;
		else if (-vx >= 4 * MARIO_WALKING_SPEED)
			ani = maxspdL;
		else if (vx > 0 && nx > 0)
			ani = walkingR;
		else if (vx < 0 && nx < 0)
			ani = walkingL;
		else if (vx > 0 && nx < 0)
			ani = breakR;
		else if (vx < 0 && nx>0)
			ani = breakL;
	}
	else
	{
		if (vy < 0 && nx > 0) ani = jumpRup;
		else if (vy > 0 && nx > 0) ani = jumpRdn;
		else if (vy < 0 && nx < 0) ani = jumpLup;
		else ani = jumpLdn;

	}
}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);
	
	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		a = MARIO_ACCELERATION + additionalforce;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		a = -(MARIO_ACCELERATION + additionalforce);
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		break;
	case MARIO_STATE_IDLE:
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	case MARIO_HOLD_JUMP:
		if (mul!=0)
		{
			vy -= MARIO_JUMP_ACCEL * dt * mul;
			if (vy <= -MARIO_MAX_JUMP_HEIGHT)
			{
				mul = 0;
			}
			slow_fall_mu1 = 1;
			isOnFloor = false;
		}
		break;

	case RACOON_TAKING_OFF:
		maximum_speed_mul = 4;
		additionalforce = a/10;
		break;

	case MARIO_NORMALIZE:
		maximum_speed_mul = 2;
		additionalforce = 0;
		break;
	case SPACE_RELEASED:
		mul = 0;
		//slow_fall_mu1 = 1;
		break;
	case SHOOT_FIREBALL:
		if (level==MARIO_LEVEL_FIRE)
			FireBall* fb = new FireBall(x, y,nx);
		break;
	
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	int temp = 0;
	top = y;
	left = x;

	if (level!=MARIO_LEVEL_SMALL && level!=MARIO_LEVEL_RACOON)
	{
		right = x +	MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
	}
	else if (level==MARIO_LEVEL_RACOON)
	{
		if (nx < 0)
		{
			right = x + MARIO_BIG_BBOX_WIDTH;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
		else if (nx > 0)
		{
			left =	x + 5;
			right = x + MARIO_BIG_BBOX_WIDTH + 5;
			bottom = y + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	height = bottom - top;
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_RACOON);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

