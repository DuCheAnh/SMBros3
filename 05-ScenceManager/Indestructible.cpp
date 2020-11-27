#include "Indestructible.h"

CINDES::CINDES(float l, float t, float r, float b)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}

CINDES::~CINDES()
{

}

void CINDES::Render()
{
	RenderBoundingBox();
}

void CINDES::GetBoundingBox(float &l,float &t, float &r, float &b)
{
	l = left;
	t = top;
	r = right;
	b = bottom;
}