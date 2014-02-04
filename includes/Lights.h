#pragma once

#include "Ray.h"
#include "BRDF.h"
#include "LocalGeo.h"
#include "Camera.h"

//Light class - abstract
class Light
{
protected:
	Point pos;
	Color lightColor;
public:
	Light():pos(Point()), lightColor(Color()) {}

	Light(Point p, Color lColor): pos(p), lightColor(lColor) {}

	virtual void generateLightRay(LocalGeo local, Ray* lray, Color* lcolor) = 0;

	virtual Color shade(LocalGeo l, BRDF b, Ray lray, Point initPos)
	{
		return Color(0.0,0.0,0.0);
	}
};

//PointLight
class PointLight : public Light
{
protected:
	vec3 attenuation;
public:

	PointLight():Light(),attenuation(vec3(1.0,0.0,0.0)) {}

	PointLight(Point p, Color lColor, vec3 at = vec3(1.0,0.0,0.0)):Light(p,lColor),attenuation(at) {}

	void generateLightRay(LocalGeo local, Ray* lray, Color* lcolor)
	{
		float x = float(randomNumber(0,1000))/1000.f;
		float y = float(randomNumber(0,1000))/1000.f;
		float theta = 2*pi*float(randomNumber(0,1000))/1000.f;
		float phi = glm::acos(2*x-1);
		float r = 0.15f*glm::pow(float(randomNumber(0,1000))/1000.f,1.f/3.f);
		Point ps = pos+Point(r*glm::cos(theta)*glm::sin(phi), r*glm::sin(theta)*glm::sin(phi), r*glm::cos(phi));
		vec3 dir = glm::normalize(ps-local.Position());
		Point p = local.Position()+eps*dir;
		*lray = Ray(p,dir,0.0,glm::distance(ps,local.Position()));
		*lcolor = Color(lightColor);
	}

	Color shade(LocalGeo l, BRDF b, Ray lray, Point initPos)
	{
		Color res = Color(0.0,0.0,0.0);
		vec3 lightVec = glm::normalize(lray.Direction());
		vec3 normal = glm::normalize(l.Norm());
		float s = glm::dot(normal,lightVec);
		Color diff = (MAX(s,0.f))*b.Diffuse()*lightColor;
		vec3 toEye = glm::normalize(initPos-l.Position());
		vec3 halfV = glm::normalize(toEye+lightVec); //glm::normalize(2*glm::dot(lightVec,normal)*normal-lightVec);
		float t = glm::dot(normal,halfV);
		Color spec = b.Specular()*lightColor*glm::pow((MAX(t,0.f)),b.Shininess());
		float d = glm::distance(pos,l.Position());
		float A = attenuation.x+attenuation.y*d+attenuation.z*d*d;
		res = (diff+spec)/A;
		return res;
	}
};

//DirectionalLight
class DirectionalLight : public Light
{
public:
	DirectionalLight():Light() {}

	DirectionalLight(Point p, Color lColor):Light(p,lColor) {}

	void generateLightRay(LocalGeo local, Ray* lray, Color* lcolor)
	{
		vec3 dir = glm::normalize(pos);
		Point p = local.Position()+eps*dir;
		*lray = Ray(p,dir,0.0,std::numeric_limits<float>::max());
		*lcolor = Color(lightColor);
	}

	Color shade(LocalGeo l, BRDF b, Ray lray, Point initPos)
	{
		Color res = Color(0.0,0.0,0.0);
		vec3 lightVec = glm::normalize(lray.Direction());
		vec3 normal = glm::normalize(l.Norm());
		float s = glm::dot(normal,lightVec);
		Color diff = (MAX(s,0.f))*b.Diffuse()*lightColor;
		vec3 toEye = glm::normalize(initPos-l.Position());
		vec3 halfV = glm::normalize(toEye+lightVec); //glm::normalize(2*glm::dot(lightVec,normal)*normal-lightVec);
		float t = glm::dot(normal,halfV);
		Color spec = b.Specular()*lightColor*glm::pow((MAX(t,0.f)),b.Shininess());
		res = diff+spec;
		return res;
	}
};