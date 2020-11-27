#include "GameObject.h"
#ifndef PLATFORM_H
#define PLATFORM_H
// some declarations in
// the header file.
class Platform : public CGameObject
{
private:
	int left, top, right, bottom;
	bool disabled = false;
	int player_height=0;
public:
	Platform(float l, float t, float r, float b);
	~Platform();
	void DisablePlatform(int height);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};
#endif


