#pragma once

#include "Shapes.h"
#include "BRDF.h"
#include "Transformation.h"
#include <vector>
using namespace std;


class Intersection;

class Primitive
{
public:
	virtual bool intersect(Ray& ray, float* thit, Intersection* in) = 0;
	virtual bool intersectP(Ray& ray) = 0;
	virtual BRDF getBRDF(const LocalGeo& local) = 0;
};

//end of Primitive class

//Intersection class

class Intersection
{
protected:
	LocalGeo local;
	Primitive* primitive;
public:
	Intersection():local(LocalGeo()),primitive(nullptr) {}
	
	Intersection(LocalGeo l, Primitive* pr):local(l),primitive(pr) {}

	Intersection(const Intersection& other)
	{
		local = other.local;
		primitive = other.primitive;
	}

	LocalGeo Local() { return local; }
	Primitive* PrimPtr() { return primitive; }

	void setLocal(LocalGeo l) { local = LocalGeo(l); }
	void setPrim(Primitive* p) { primitive = p; }
};

//Geometric Primitive
class GeometricPrimitive : public Primitive
{
protected:
	Material material;
	Transformation objToWorld;
public:
	GeometricPrimitive():material(Material()), objToWorld(Transformation()) {}

	GeometricPrimitive(Material mat, Transformation obj):material(mat), objToWorld(obj){}

	BRDF getBRDF(const LocalGeo& local)
	{
		return material.getBRDF(local);
	}
};

//Primitives

class TriangleObj : public GeometricPrimitive
{
protected:
	Triangle shape;
public:
	TriangleObj():GeometricPrimitive(), shape(Triangle()) {}

	TriangleObj(Point p0, Point p1, Point p2, Material m, Transformation obj):GeometricPrimitive(m,obj)
	{
		shape = Triangle(p0, p1, p2);
	}

	bool intersect(Ray& ray, float* thit, Intersection* in)
	{
		Ray oray = objToWorld.transformRay(ray);
		LocalGeo local;
		if(!shape.intersect(oray,thit,&local)) return false;
		in->setPrim(this);
		in->setLocal(objToWorld.transformLocalGeo(local));
		return true;
	}

	bool intersectP(Ray& ray)
	{
		Ray oray = objToWorld.transformRay(ray);
		return shape.intersectP(oray);
	}
};

class SphereObj : public GeometricPrimitive
{
protected:
	Sphere shape;
public:
	SphereObj():GeometricPrimitive(), shape(Sphere()) {}

	SphereObj(Point c, float r, Material m, Transformation obj):GeometricPrimitive(m,obj)
	{
		shape = Sphere(c,r);
	}

	bool intersect(Ray& ray, float* thit, Intersection* in)
	{
		Ray oray = objToWorld.transformRay(ray);
		LocalGeo local;
		if(!shape.intersect(oray,thit,&local)) return false;
		in->setPrim(this);
		in->setLocal(objToWorld.transformLocalGeo(local));
		return true;
	}

	bool intersectP(Ray& ray)
	{
		Ray oray = objToWorld.transformRay(ray);
		return shape.intersectP(oray);
	}
};


class TriangleNormalObj : public GeometricPrimitive
{
protected:
	TriangleNormal shape;
public:
	TriangleNormalObj():GeometricPrimitive(), shape(TriangleNormal()) {}

	TriangleNormalObj(Point p0, Point p1, Point p2, vec3 n0, vec3 n1, vec3 n2, Material m, Transformation obj):GeometricPrimitive(m,obj)
	{
		shape = TriangleNormal(p0, p1, p2, n0, n1, n2);
	}

	bool intersect(Ray& ray, float* thit, Intersection* in)
	{
		Ray oray = objToWorld.transformRay(ray);
		LocalGeo local;
		if(!shape.intersect(oray,thit,&local)) return false;
		in->setPrim(this);
		in->setLocal(objToWorld.transformLocalGeo(local));
		return true;
	}

	bool intersectP(Ray& ray)
	{
		Ray oray = objToWorld.transformRay(ray);
		return shape.intersectP(oray);
	}
};


//PrimitiveList class

class PrimitiveList : Primitive
{
protected:
	vector<Primitive*> pList;
public:
	PrimitiveList() {}

	PrimitiveList(vector<Primitive*> list)
	{
		pList = vector<Primitive*>(list.size());
		copy(list.begin(), list.end(), pList.begin());
	}

	vector<Primitive*> List() { return pList; }

	bool intersect(Ray& ray, float* thit, Intersection* in)
	{
		float t_min = std::numeric_limits<float>::max();
		bool intersected = false;
		for(unsigned int i=0;i<pList.size();i++)
		{
			float t;
			Intersection inn;
			if(pList[i]->intersect(ray,&t,&inn))
			{
				if(t<t_min && t>=ray.Min() && t<=ray.Max())
				{
					intersected = true;
					*in = inn;
					t_min = t;
				}
			}
		}
		*thit = t_min;
		if(!intersected)
			return false;
		return true;
	}

	bool intersectP(Ray& ray)
	{
		for(unsigned int i=0;i<pList.size();i++)
		{
			if(pList[i]->intersectP(ray))
				return true;
		}
		return false;
	}

	BRDF getBRDF(const LocalGeo& local)
	{
		return BRDF();
	}

	void addPrimitive(Primitive* p)
	{
		pList.push_back(p);
	}
};