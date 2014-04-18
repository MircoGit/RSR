#include "stdafx.h"
#include "Vector2D.h"

bool operator == ( const Vector2D& v1, const Vector2D& v2)
{
	return ((v1.x == v2.x) && (v1.y == v2.y));
}