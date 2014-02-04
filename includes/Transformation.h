#pragma once

#include "typeDefs.h"
using namespace std;


class Transformation
{
protected:
	mat4 mat, matInv, matInvT;
public:
	Transformation()
	{
		mat = matInv = matInvT = mat4(1.0);
	}

	Transformation(mat4 tr)
	{
		mat = mat4(tr);
		matInv = glm::inverse(mat);
		matInvT = glm::transpose(matInv);
	}

	vec3 transformVector(const vec3& Vec)
	{
		return vec3(vec4(Vec,0.0)*mat);
	}

	Point transformPoint(const Point& pt)
	{
		return Point(vec4(pt,1.0)*mat);
	}

	vec3 transformInverseVector(const vec3& Vec)
	{
		return vec3(vec4(Vec,0.0)*matInv);
	}

	Point transformInversePoint(const Point& pt)
	{
		return Point(vec4(pt,1.0)*matInv);
	}


	vec3 transformNormal(const vec3& normal)
	{
		vec3 x = vec3(vec4(normal,0.0)*matInvT);
		//x = glm::normalize(x);
		return x;
	}

	Ray transformRay(const Ray& ray)
	{
		Ray r = Ray(ray);
		r.setDir(transformInverseVector(r.Direction()));
		r.setPos(transformInversePoint(r.Position()));
		return r;
	}

	LocalGeo transformLocalGeo(const LocalGeo& local)
	{
		LocalGeo l = LocalGeo(local);
		l.setNormal(glm::normalize(transformNormal(l.Norm())));
		l.setPos(transformPoint(l.Position()));
		return l;
	}
};