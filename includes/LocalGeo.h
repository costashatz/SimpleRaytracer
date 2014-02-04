#pragma once

#include "typeDefs.h"

//LocalGeo class

class LocalGeo
{
protected:
	Point pos;
	vec3 normal;
public:
	LocalGeo() {}

	LocalGeo(Point p, vec3 n):pos(p), normal(n) {}

	LocalGeo(const LocalGeo& other)
	{
		pos = other.pos;
		normal = other.normal;
	}

	vec3 Position() { return pos; }
	vec3 Norm() { return normal; }

	void setPos(vec3 p) { pos = p; }
	void setNormal(vec3 n) { normal = n; }

};