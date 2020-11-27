#ifndef _COIN_H
#define _COIN_H



#include "GameObject.h"

#define DEFAULT_ANI 0

#define BBCOIN_WIDTH 16
#define BBCOIN_HEIGHT 16

class CCoin : public CGameObject
{
public:
	bool gotnomed = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Render();
	void Consumed();

};
#endif // !_CO
