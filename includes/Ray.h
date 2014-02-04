#pragma once

#include "typeDefs.h"

class Ray
{
protected:
	Point pos;
	vec3 dir;
	float t_min, t_max;
public:
	Ray() {}

	Ray(Point p, vec3 d, float tmin, float tmax) : pos(p), dir(d), t_min(tmin), t_max(tmax) {}

	Ray(const Ray& other): pos(other.pos), dir(other.dir), t_min(other.t_min), t_max(other.t_max) {}

	vec3 Position() { return pos; }
	vec3 Direction() { return dir; }
	float Min() { return t_min; }
	float Max() { return t_max; }

	void setPos(vec3 p) { pos = p; }
	void setDir(vec3 d) { dir = d; }
	void setMin(float m) { t_min = m; }
	void setMax(float m) { t_max = m; }

	//friend Ray operator*(const mat4&,const Ray&);
};

/*Ray operator*(const mat4& m,const Ray& r)
{
	Ray ray = Ray(r);
	ray.pos = vec3(m*vec4(r.pos,1.f));
	ray.dir = vec3(m*vec4(r.dir,0.f));
	ray.dir = glm::normalize(ray.dir); //isws oxi
	return ray;
}*/