#ifndef KOOPAS_H
#define	KOOPAS_H


#include "GameObject.h"

#define KOOPAS_WALKING_SPEED 0.05f;
#define KOOPAS_SHELL_SPEED	0.2f
#define KOOPAS_DEFLECT_FORCE 0.35f

#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_DIE 16

#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_DIE 200
#define KOOPAS_STATE_SHELL 300
#define KOOPAS_STATE_SHELL_MOVING	400

#define KOOPAS_ANI_WALKING_LEFT 0
#define KOOPAS_ANI_WALKING_RIGHT 1
#define KOOPAS_ANI_DIE 2

class CKoopas : public CGameObject
{
	bool shellBounce = false;
	bool isDead = false;
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

public:
	CKoopas();
	virtual void SetState(int state);
};
#endif // !KOOPAS_H
