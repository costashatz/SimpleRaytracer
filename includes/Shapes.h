#pragma once

#include "LocalGeo.h"
#include "Ray.h"

//Shape class

class Shape
{
public:
	virtual bool intersect(Ray& ray, float* thit, LocalGeo* local) { return false; }
	virtual bool intersectP(Ray& ray)
	{
		float temp;
		LocalGeo l;
		return intersect(ray,&temp,&l);
	}
};


//Triangle
class Triangle : public Shape
{
private:
	Point p0,p1,p2;
public:
	Triangle() {}

	Triangle(Point _p0, Point _p1, Point _p2)
	{
		p0 = _p0;
		p1 = _p1;
		p2 = _p2;
	}

	bool intersect(Ray& ray, float* thit, LocalGeo* local)
	{
		vec3 e1 = p1-p0, e2 = p2-p0, p, s, q;
		float tmp, t, u, v, epsilon = 0.000001;
		p = glm::cross(ray.Direction(),e2);
		tmp = glm::dot(p,e1);
		if(tmp>-epsilon && tmp < epsilon)
			return false;
		tmp = 1.f/tmp;
		s = ray.Position()-p0;
		u = tmp*glm::dot(s,p);
		if(u<0.0 || u>1.0)
			return false;
		q = glm::cross(s,e1);
		v = tmp*glm::dot(ray.Direction(),q);
		if(v<0.0 || u+v>1.0)
			return false;
		t = tmp*glm::dot(e2,q);
		Point P = ray.Position()+t*ray.Direction();
		vec3 n = glm::normalize(glm::cross(e1,e2));
		if(t<ray.Min() || t>ray.Max())
			return false;
		*thit = t;
		*local = LocalGeo(P, n);
		return true;
	}
};

//Sphere
class Sphere : public Shape
{
private:
	Point center;
	float radius;
public:
	Sphere():center(Point()),radius(0.0) {}
	Sphere(Point c, float r):center(c),radius(r) {}

	bool intersect(Ray& ray, float* thit, LocalGeo* local)
	{
		float a,b,c, discrm, t;
		vec3 pMinusC = ray.Position()-center;
		a = glm::dot(ray.Direction(),ray.Direction());
		b = 2.0*glm::dot(ray.Direction(),pMinusC);
		c = glm::dot(pMinusC,pMinusC)-radius*radius;
		discrm = b*b-4*a*c;
		if(discrm<0.f)
			return false;
		float t1 = (-b+sqrt(discrm))/(2*a);
		float t2 = (-b-sqrt(discrm))/(2*a);
		if(t1>=0.0&&t2>=0.0)
		{
			t = t1;
			if(t2<t1)
				t = t2;
		}
		else
		{
			if(t1>=0.0)
				t = t1;
			else if(t2>=0.0)
				t = t2;
			else
				return false;
		}
		if(t<ray.Min() || t>ray.Max())
			return false;
		Point temp = ray.Position()+t*ray.Direction();
		vec3 n = glm::normalize(temp-center);
		*thit = t;
		*local = LocalGeo(temp, n);
		return true;
	}
};

//TriangleNormal
class TriangleNormal : public Shape
{
private:
	Point p0,p1,p2;
	vec3 n0,n1,n2;
public:
	TriangleNormal() {}

	TriangleNormal(Point _p0, Point _p1, Point _p2, vec3 _n0, vec3 _n1, vec3 _n2)
	{
		p0 = _p0;
		p1 = _p1;
		p2 = _p2;
		n0 = _n0;
		n1 = _n1;
		n2 = _n2;
	}

	bool intersect(Ray& ray, float* thit, LocalGeo* local)
	{
		vec3 e1 = p1-p0, e2 = p2-p0, p, s, q;
		float tmp, t, u, v, epsilon = 0.000001;
		p = glm::cross(ray.Direction(),e2);
		tmp = glm::dot(p,e1);
		if(tmp>-epsilon && tmp < epsilon)
			return false;
		tmp = 1.f/tmp;
		s = ray.Position()-p0;
		u = tmp*glm::dot(s,p);
		if(u<0.0 || u>1.0)
			return false;
		q = glm::cross(s,e1);
		v = tmp*glm::dot(ray.Direction(),q);
		if(v<0.0 || u+v>1.0)
			return false;
		t = tmp*glm::dot(e2,q);
		Point P = ray.Position()+t*ray.Direction();
		float w = 1-u-v;
		vec3 n = glm::normalize(w*n0+u*n1+v*n2);
		if(t<ray.Min() || t>ray.Max())
			return false;
		*thit = t;
		*local = LocalGeo(P, n);
		return true;
	}
};