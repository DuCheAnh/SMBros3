#pragma once
#ifndef MARIO_H
#define MARIO_H

#include "GameObject.h"

#define MARIO_WALKING_SPEED		0.05f 
#define MARIO_ACCELERATION		0.00025f
#define MARIO_JUMP_ACCEL		0.002f
//0.1f
#define MARIO_MAX_JUMP_HEIGHT	0.23f
#define MARIO_JUMP_SPEED_Y		0.005f
#define MARIO_JUMP_DEFLECT_SPEED 0.2f
#define MARIO_GRAVITY			0.0008f
#define MARIO_DIE_DEFLECT_SPEED	 0.5f

#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200
#define MARIO_STATE_JUMP			300
#define MARIO_STATE_DIE				400
#define MARIO_HOLD_JUMP				500
#define RACOON_TAKING_OFF			600
#define MARIO_NORMALIZE				700
#define SPACE_RELEASED				800
#define SHOOT_FIREBALL				900
#pragma region Big ani
#define MARIO_ANI_BIG_IDLE_RIGHT		0
#define MARIO_ANI_BIG_IDLE_LEFT			1

#define MARIO_ANI_BIG_WALKING_RIGHT			4
#define MARIO_ANI_BIG_WALKING_LEFT			5

#define MARIO_ANI_BIG_BREAK_LEFT	9
#define MARIO_ANI_BIG_BREAK_RIGHT	10

#define MARIO_ANI_BIG_JUMP_RIGHTUP 11
#define MARIO_ANI_BIG_JUMP_RIGHTDN 12
#define MARIO_ANI_BIG_JUMP_LEFTUP 13
#define MARIO_ANI_BIG_JUMP_LEFTDN 14
#pragma endregion

#pragma region Small ani
#define MARIO_ANI_SMALL_IDLE_RIGHT		2
#define MARIO_ANI_SMALL_IDLE_LEFT			3

#define MARIO_ANI_SMALL_WALKING_RIGHT		6
#define MARIO_ANI_SMALL_WALKING_LEFT		7
#define MARIO_ANI_SMALL_BREAK_LEFT	15
#define MARIO_ANI_SMALL_BREAK_RIGHT	16

#define MARIO_ANI_SMALL_JUMP_RIGHTUP 17
#define MARIO_ANI_SMALL_JUMP_RIGHTDN 18
#define MARIO_ANI_SMALL_JUMP_LEFTUP 19
#define MARIO_ANI_SMALL_JUMP_LEFTDN 20
#pragma endregion

#pragma region Fire ani
#define MARIO_ANI_FIRE_IDLE_RIGHT 21
#define MARIO_ANI_FIRE_IDLE_LEFT 22

#define MARIO_ANI_FIRE_WALKING_RIGHT 23
#define MARIO_ANI_FIRE_WALKING_LEFT 24
#define MARIO_ANI_FIRE_BREAK_LEFT 25
#define MARIO_ANI_FIRE_BREAK_RIGHT 26
				 
#define MARIO_ANI_FIRE_JUMP_RIGHTUP 27
#define MARIO_ANI_FIRE_JUMP_RIGHTDN 28
#define MARIO_ANI_FIRE_JUMP_LEFTUP 29
#define MARIO_ANI_FIRE_JUMP_LEFTDN 30
#pragma endregion

#pragma region Racoon ani
#define MARIO_ANI_RACOON_IDLE_RIGHT 31
#define MARIO_ANI_RACOON_IDLE_LEFT 32

#define MARIO_ANI_RACOON_WALKING_RIGHT 33
#define MARIO_ANI_RACOON_WALKING_LEFT 34
#define MARIO_ANI_RACOON_BREAK_LEFT 35
#define MARIO_ANI_RACOON_BREAK_RIGHT 36

#define MARIO_ANI_RACOON_JUMP_RIGHTUP 37
#define MARIO_ANI_RACOON_JUMP_RIGHTDN 38
#define MARIO_ANI_RACOON_JUMP_LEFTUP 39
#define MARIO_ANI_RACOON_JUMP_LEFTDN 40
#pragma endregion



#define MARIO_ANI_DIE				8

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_FIRE	3
#define MARIO_LEVEL_RACOON	4
#define MARIO_BIG_BBOX_WIDTH  15
#define MARIO_BIG_BBOX_HEIGHT 27

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 15

#define MARIO_UNTOUCHABLE_TIME 5000


class CMario : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;
	int height=0;
	int slow_fall_mu1 = 1;
	int mul = 1;
	int maximum_speed_mul = 2;
	float additionalforce=0;
	bool isOnFloor;
	bool isFlying = false;
	bool canFly = false;
	float a,ay;
	float start_x;			// initial position of Mario at scene
	float start_y; 
public: 
	CMario(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	virtual void Render();

	void SetState(int state);
	void SetLevel(int l) { level = l; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void StateRenderding(int& ani, int idleR, int idleL, int walkingR, int walkingL, int breakR, int breakL,
						int jumpRup, int jumpRdn, int jumpLup, int jumpLdn);
	void Reset();
	int GetHeight() { return height; }
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	int GetLevel() { return level; }
};
#endif // !MARIO_H
