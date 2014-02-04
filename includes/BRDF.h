#pragma once

#include "typeDefs.h"

class LocalGeo;

//BRDF class
class BRDF
{
protected:
	Color diffuse, specular, ambient, emissive, mirror, refract;
	float shininess, rTheta;
public:
	BRDF():diffuse(Color()), specular(Color()), ambient(Color()), mirror(Color()), emissive(Color()), refract(Color()), shininess(1.0), rTheta(0.0) {}

	BRDF(Color kd, Color ks, Color ka, Color ke, float s = 1.0, Color km = Color(0.0,0.0,0.0), Color kr = Color(0.0,0.0,0.0), float theta = 0.0):diffuse(kd), specular(ks), ambient(ka), mirror(km), emissive(ke), refract(kr), shininess(s), rTheta(theta) {}

	BRDF(const BRDF& other)
	{
		diffuse = other.diffuse;
		specular = other.specular;
		ambient = other.ambient;
		mirror = other.mirror;
		emissive = other.emissive;
		shininess = other.shininess;
		refract = other.refract;
		rTheta = other.rTheta;
	}

	Color Diffuse() { return diffuse; }
	Color Specular() { return specular; }
	Color Ambient() { return ambient; }
	Color Mirror() { return mirror; }
	Color Refraction() { return refract; }
	Color Emissive() { return emissive; }
	float Shininess() { return shininess; }
	float Theta() { return rTheta; }

	void setValues(Color kd, Color ks, Color ka, Color ke, float s = 1.0, Color km = Color(0.0,0.0,0.0), Color kr = Color(0.0,0.0,0.0), float theta = 0.0)
	{
		diffuse = kd;
		specular = ks;
		ambient = ka;
		mirror = km;
		emissive = ke;
		refract = kr;
		shininess = s;
		rTheta = theta;
	}

	void setAmbient(Color c) { ambient = c; }
	void setDiffuse(Color c) { diffuse = c; }
	void setSpecular(Color c) { specular = c; }
	void setEmissive(Color c) { emissive = c; }
	void setMirror(Color c) { mirror = c; }
	void setRefraction(Color c) { refract = c; }
	void setTheta(float r) { rTheta = r; }
	void setShininess(float s) { shininess = s; }
};

//Material class

class Material
{
protected:
	BRDF mat;
public:
	Material():mat(BRDF()) {}

	Material(BRDF m):mat(m) {}

	Material(const Material& other)
	{
		mat = BRDF(other.mat);
	}

	void setBRDF(BRDF m)
	{
		mat = m;
	}

	BRDF getBRDF(const LocalGeo& local)
	{
		return mat;
	}
};