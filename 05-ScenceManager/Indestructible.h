#include "GameObject.h"
class CINDES : public CGameObject
{
private:
	int left, top, right, bottom;
public:
	CINDES(float l,float t,float r,float b);
	~CINDES();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

