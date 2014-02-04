#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef glm::mat3 mat3 ;
typedef glm::mat4 mat4 ; 
typedef glm::vec3 vec3 ; 
typedef glm::vec3 Color;
typedef glm::vec3 Point;
typedef glm::vec4 vec4 ; 
const float pi = 3.14159265; // For portability across platforms
const float eps = 0.001;
const float e = 2.71828182846;


#define clamp(x,a,b) (x < a ? a : (x > b ? b : x))
#define MAX(a,b) a>b?a:b
#define SAMPLES 1

using namespace std;


int randomNumber(int lowest, int highest)
{
	int range=(highest-lowest)+1;
	return lowest+int(range*rand()/(RAND_MAX + 1.0));
}