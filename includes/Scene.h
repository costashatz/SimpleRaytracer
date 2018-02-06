#pragma once

#include <tbb/parallel_for.h>

#include "Camera.h"
#include "Primitives.h"
#include "Lights.h"
#include "Film.h"

#define MAX_DEPTH 5

//RayTracer and Scene classes
class Scene;

class RayTracer
{
private:
	Scene* scene;
	int max_depth;
public:
	RayTracer():scene(nullptr),max_depth(0) {}
	RayTracer(Scene* s, int m=MAX_DEPTH):scene(s),max_depth(m) {}

	void trace(Ray ray, int depth, Color* color);

	Ray createReflectRay(const LocalGeo& local, const Ray& ray);
};


class Scene
{
protected:
	PrimitiveList primitives;
	vector<Light*> lights;
	Film film;
	RayTracer raytracer;
	Camera camera;
	string file;
public:
	Scene(string f = "final.png")
	{
		primitives = PrimitiveList();
		file = f;
	}

	void setParameters(Camera cam, int mDepth=MAX_DEPTH)
	{
		camera = Camera(cam);
		int w, h;
		camera.getViewPort(&w,&h);
		film = Film(w,h);
		raytracer = RayTracer(this,mDepth);
	}

	Camera getCamera() { return camera; }

	void addPrimitive(Primitive* p)
	{
		primitives.addPrimitive(p);
	}

	void addLight(Light* l)
	{
		lights.push_back(l);
	}

	void setOutput(string s)
	{
		file = s;
	}

	void render()
	{
		int num = camera.Width()*camera.Height();
		// float p = 0.f;
		// for(unsigned int r=0;r<camera.Width();r++)
		tbb::parallel_for(size_t(0), size_t(camera.Width()), size_t(1), [&](size_t r) {
			// for(unsigned int c=0;c<camera.Height();c++)
			tbb::parallel_for(size_t(0), size_t(camera.Height()), size_t(1), [&](size_t c) {
				Color color = Color(0.f,0.f,0.f);
				for(unsigned int i=0;i<SAMPLES;i++)
				{
					for(unsigned int j=0;j<SAMPLES;j++)
					{
						Color cs = Color(0.f,0.f,0.f);
						float ks = (float(randomNumber(0,1000))/1000.f);
						float addX = (float(i)+ks)/float(SAMPLES);
						float addY = (float(j)+ks)/float(SAMPLES);
						Ray ray = camera.generateRay(r,c,addX,addY);
						raytracer.trace(ray,1,&cs);
						color += cs;
					}
				}
				color /= float(SAMPLES*SAMPLES);
				film.Commit(r,c,color);
				// p = 100.f*float(r*camera.Width()+c)/float(num);
				// if(p==10.f || p == 30.f || p == 50.f || p == 70.f || p == 80.f || p == 85.f || p == 90.f || p == 95.f || p == 97.f || p == 98.f || p == 99.f || p == 100.f)
				// {
				// 	cout<<p<<"%"<<endl;
				// }
			});
		});
		// cout<<"Writing to file...\n";
		film.WriteImage(file);
		file = "xdg-open "+file;
		system(file.c_str());
	}

	PrimitiveList Primitives() { return primitives;}
	vector<Light*> Lights() { return lights; }
};

void RayTracer::trace(Ray ray, int depth, Color* color)
{
	*color = Color(0.0,0.0,0.0);
	if(depth>=max_depth)
	{
		return;
	}
	float thit = 0.0;
	Intersection* in = new Intersection();
	if(!scene->Primitives().intersect(ray,&thit,in))
	{
		return;
	}
	BRDF brdf = in->PrimPtr()->getBRDF(in->Local());

	*color += brdf.Ambient() + brdf.Emissive();

	vector<Light*> lights = scene->Lights();
	for(unsigned int i=0;i<lights.size();i++)
	{
		Ray lightRay;
		Color lightColor, lColor = Color(0.f,0.f,0.f);
		for(unsigned int k=0;k<SAMPLES;k++)
		{
			lights[i]->generateLightRay(in->Local(),&lightRay,&lightColor);
			if(!scene->Primitives().intersectP(lightRay))
			{
				lColor += lights[i]->shade(in->Local(), brdf, lightRay, ray.Position());
			}
		}
		*color += (lColor/float(SAMPLES));
	}
	float R = 1.f;
	Color k = Color(1.f,1.f,1.f);
	/*if(brdf.Refraction().length()>eps)
	{
		vec3 d = ray.Direction();
		vec3 n = glm::normalize(in->Local().Norm());
		float t = brdf.Theta();
		vec3 dir;
		Ray refractRay;
		float c = glm::dot(d,n);
		if(c<0)
		{
			c = -c;
			dir = glm::refract(d,n,t);
		}
		else
		{
			dir = glm::refract(d,-n,1.f/t);
			c = glm::dot(dir,n);
			Color a = Color(0.5, 0.5, 0.5);
			float l1 = glm::exp(-glm::log(a.x)*thit);
			float l2 = glm::exp(-glm::log(a.y)*thit);
			float l3 = glm::exp(-glm::log(a.z)*thit);
			k = Color(l1,l2,l3);
		}
		refractRay = Ray(in->Local().Position()+eps*dir, dir, 0.0, std::numeric_limits<float>::max()); //createReflectRay(in->Local(), ray);
		Color tempCol;
		if(glm::length(dir)>eps)
		{
			trace(refractRay,depth+1,&tempCol);
			float R0 = glm::pow(t-1,2.f)/glm::pow(t+1,2.f);
			R = R0+(1-R0)*glm::pow(1-c,5.f);
			*color += k*(1-R)*tempCol;
		}
	}*/
	if(brdf.Specular().length()>eps)
	{
		vec3 dir = glm::reflect(ray.Direction(),glm::normalize(in->Local().Norm()));
		Ray reflectRay = Ray(in->Local().Position()+eps*dir, dir, 0.0, std::numeric_limits<float>::max()); //createReflectRay(in->Local(), ray);
		Color tempCol;
		trace(reflectRay,depth+1,&tempCol);
		*color += k*brdf.Specular()*tempCol;
	}
}

Ray RayTracer::createReflectRay(const LocalGeo& local, const Ray& ray)
{
	return Ray(ray);
}