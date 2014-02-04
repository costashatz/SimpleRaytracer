#pragma once

#include "typeDefs.h"
#include "Ray.h"

//Camera class

class Camera
{
private:
	vec3 pos, center, up;
	vec3 u,v,w;
	float fovy, fovx;
	int width, height;
public:
	Camera() {}

	Camera(vec3 p, vec3 c, vec3 _up, float fy): pos(p), center(c), up(_up)
	{
		w = glm::normalize(p-c);
		u = glm::normalize(glm::cross(_up,w));
		v = glm::cross(w,u);
		fovy = fy*pi/180.f;
	}

	Camera(const Camera& other)
	{
		pos = other.pos;
		center = other.center;
		up = other.up;
		u = other.u;
		v = other.v;
		w = other.w;
		fovy = other.fovy;
		fovx = other.fovx;
		width = other.width;
		height = other.height;
	}

	int Width() { return width; }
	int Height() { return height; }

	void setViewPort(int wi, int he)
	{
		width = wi;
		height = he;
		fovx = 2*atan(tan(fovy/2.f)*(float(wi)/float(he)));
	}

	void getViewPort(int* wi, int* he)
	{
		*he = height;
		*wi = width;
	}

	void getParameters(vec3* p, vec3* c, vec3* _up, float* fy, float* fx)
	{
		*fy = fovy;
		*fx = fovx;
		*p = pos;
		*c = center;
		*_up = up;
	}

	Ray generateRay(unsigned int _x, unsigned int _y, float addX = 0.5, float addY = 0.5)
	{
		float x = float(_x)+addX;
		float y = float(_y)+addY;
		float f1 = fovx;
		float f2 = fovy;
		float W = float(width)/2.f;
		float h = float(height)/2.f;
		float a = tan(f1/2.f)*(x-W)/W;
		float b = tan(f2/2.f)*(h-y)/h;
		vec3 dir = glm::normalize(a*u+b*v-w);
		Ray r = Ray(pos, dir, 0.0, std::numeric_limits<float>::max());
		return r;
	}
};