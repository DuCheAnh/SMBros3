#include "GameObject.h"
#include "coin.h"
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16
#define STATE_POPPED	1
#define STATE_DEFAULT	0
class CMysterious : public CGameObject
{
	CCoin c;
public:
	CMysterious();
	virtual void Render();
	virtual void GotPopped();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
}; 
